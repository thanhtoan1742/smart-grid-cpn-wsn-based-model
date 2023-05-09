#include <sgrid/State.h>

#include <sgrid/Outcome.h>
#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/TransmissionLineState.h>
#include <sgrid/Types.h>
#include <sgrid/utils.h>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <plog/Log.h>

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits.h>
#include <map>
#include <numeric>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace sgrid {

State::State(Grid* _grid)
    : grid(_grid),
      outcomes(_grid->tls.size(), nullptr),
      depth(0),
      parent(nullptr) {
  // TODO: only create state for GEN and CON
  for (auto ps: grid->pss) {
    if (ps->pst != PowerSystemType::Generator &&
        ps->pst != PowerSystemType::Consumer)
      continue;
    psStates.push_back(new PowerSystemState(ps));
  }
  for (auto tl: grid->tls)
    tlStates.push_back(new TransmissionLineState(tl));
}

State::State(State const& original)
    : grid(original.grid),
      outcomes(original.grid->tls.size(), nullptr),
      depth(0),
      parent(nullptr) {
  for (auto psState: original.psStates)
    psStates.push_back(new PowerSystemState(*psState));
  for (auto tlState: original.tlStates)
    tlStates.push_back(new TransmissionLineState(*tlState));
}

State::~State() {
  for (auto psState: psStates)
    delete psState;
  for (auto tlState: tlStates)
    delete tlState;
  for (auto outcome: outcomes)
    delete outcome;
}

bool State::operator==(State const& other) const& {
  if (psStates.size() != other.psStates.size() ||
      tlStates.size() != other.tlStates.size())
    return false;
  for (int i = 0; i < psStates.size(); ++i)
    if (*psStates[i] != *other.psStates[i])
      return false;
  for (int i = 0; i < tlStates.size(); ++i)
    if (*tlStates[i] != *other.tlStates[i])
      return false;
  return true;
}

bool State::satisfied() const& {
  for (auto psState: psStates) {
    if (psState->ps->pst == PowerSystemType::Consumer &&
        psState->used < psState->keeping)
      return false;
    if (psState->keeping != 0)
      return false;
  }
  return true;
}

Power State::notDemaned() const& {
  Power res(0);
  for (auto const& psState: psStates)
    if (psState->ps->pst == PowerSystemType::Consumer)
      res += psState->ps->capacity - psState->used;
  return res;
}

Power State::fulfilled() const& {
  Power res(0);
  for (auto const& psState: psStates)
    if (psState->ps->pst == PowerSystemType::Generator)
      res += psState->used;
  return res;
}

Power State::needFulfilled() const& {
  Power res(0);
  for (auto const& psState: psStates)
    if (psState->ps->pst == PowerSystemType::Generator)
      res += std::max(psState->keeping - psState->fulfillable(), 0_pu);
  return res;
}

Power State::keeping() const& {
  Power res(0);
  for (auto const& psState: psStates)
    res += psState->keeping;
  return res;
}

void State::demand() {
  for (auto psState: psStates)
    if (psState->ps->pst == PowerSystemType::Consumer) {
      psState->keeping = psState->ps->capacity;
      psState->used    = psState->ps->capacity;
    }
}

std::vector<State> State::generateNextStates() {
  // PLOGD << "GENERATING NEXT STATES";
  calculateOutcomes();

  std::unordered_set<State> uniqueStates;
  for (auto tl: grid->tls) {
    Outcome* outcome = outcomes[tl->id];
    if (!outcome)
      continue;
    PowerSystem* inp = tl->inp;
    if (inp->pst != PowerSystemType::Consumer)
      continue;
    PowerSystem* out  = outcome->gen;
    Percentage   loss = outcome->loss;
    if (psStates[inp->id]->keeping == 0) {
      continue;
    }

    Power amount = std::min(outcome->fulfillable(), psStates[inp->id]->keeping);

    State nextState(*this);

    Outcome* current = outcome;
    // PLOGD << " PROCESSING OUTCOME";
    nextState.psStates[inp->id]->send(amount);
    while (current != nullptr) {
      // PLOGD << " TRACING " << current->toString();
      auto tlState = nextState.tlStates[current->tl->id];
      tlState->transmit(amount);
      amount  = amount * current->tl->loss;
      current = current->trace;
    }
    nextState.psStates[out->id]->receive(amount);
    nextState.psStates[out->id]->fulfill();
    uniqueStates.insert(nextState);
  }

  PLOGD << "NEXT STATES";
  for (auto const& state: uniqueStates)
    PLOGD << state;

  return std::vector<State>(
      std::make_move_iterator(uniqueStates.begin()),
      std::make_move_iterator(uniqueStates.end())
  );
}

using PSO = std::pair<PowerSystem*, Outcome*>;

struct LossComparator {
  bool operator()(PSO const& a, PSO const& b) {
    return a.second->loss > b.second->loss;
  }
};

void State::calculateOutcomes() {
  std::priority_queue<PSO, std::vector<PSO>, LossComparator> q;
  std::vector<Outcome*> bestOutcome(grid->pss.size(), nullptr);

  for (auto tl: grid->tls) {
    auto gen = tl->out;
    auto inp = tl->inp;
    if (gen->pst != PowerSystemType::Generator)
      continue;
    if (psStates[gen->id]->fulfillable() == 0)
      continue;
    outcomes[tl->id] =
        new Outcome(this, tl, gen, tl->loss, tl, 100_pct, nullptr);
    bestOutcome[inp->id] = outcomes[tl->id];
    q.emplace(tl->inp, bestOutcome[inp->id]);
  }

  while (!q.empty()) {
    auto [ps, outcome] = q.top();
    q.pop();
    if (bestOutcome[ps->id] != outcome)
      continue;

    for (TransmissionLine* tl: ps->revAdj) {
      auto inp = tl->inp;

      TransmissionLine* congestedTl     = outcome->congestedTl;
      Percentage        congestedTlLoss = outcome->congestedTlLoss * tl->loss;
      if (tl->capacity < congestedTl->capacity / congestedTlLoss) {
        congestedTl     = tl;
        congestedTlLoss = 100_pct;
      }
      outcomes[tl->id] = new Outcome(
          this,
          tl,
          outcome->gen,
          outcome->loss * tl->loss,
          congestedTl,
          congestedTlLoss,
          outcome
      );

      if (outcomes[tl->id]->fulfillable() == 0)
        continue;

      if (bestOutcome[inp->id] == nullptr ||
          bestOutcome[inp->id]->loss > outcomes[tl->id]->loss) {
        bestOutcome[inp->id] = outcomes[tl->id];
        q.emplace(inp, bestOutcome[inp->id]);
      }
    }
  }
}

std::string State::toString() const& {
  if (psStates.empty())
    return "[]";

  std::stringstream ss;
  ss << "[";
  for (auto psState: psStates) {
    ss << fmt::format(stateElementFormat, psState->toString());
  }
  ss << "]";
  ss << "[";
  for (auto tlState: tlStates) {
    ss << fmt::format("{:<4}", tlState->toString());
  }
  ss << "]";
  return ss.str();
}

} // namespace sgrid

std::size_t std::hash<sgrid::State>::operator()(sgrid::State const& state
) const noexcept {
  return std::hash<std::string>()(state.toString());
}

namespace plog {
Record& operator<<(Record& record, sgrid::State const& state) {
  static std::map<std::string, int> stateId;

  std::string str = state.toString();
  int         id  = -1;
  auto        it  = stateId.find(str);
  if (it == stateId.end()) {
    id           = stateId.size();
    stateId[str] = id;
  } else
    id = it->second;

  record << fmt::format("{} __ID_{}_ID__", str, id);

  return record;
}
} // namespace plog