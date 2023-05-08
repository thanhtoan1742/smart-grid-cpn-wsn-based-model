#include <sgrid/State.h>

#include <sgrid/Outcome.h>
#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/TransmissionLine.h>
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
    : grid(_grid), depth(0), parent(nullptr), outcomes(_grid->tls.size()) {
  for (auto ps: grid->pss)
    psStates.emplace_back(ps);
}

bool State::operator==(State const& other) const& {
  if (psStates.size() != other.psStates.size())
    return false;
  for (int i = 0; i < psStates.size(); ++i)
    if (psStates[i] != other.psStates[i])
      return false;
  return true;
}

bool State::satisfied() const& {
  for (int i = 0; i < grid->pss.size(); ++i) {
    PowerSystem* ps = grid->pss[i];
    if (ps->pst == PowerSystemType::Consumer && psStates[i].used < ps->capacity)
      return false;
    if (psStates[i].keeping != 0)
      return false;
  }
  return true;
}

Power State::notDemaned() const& {
  Power res(0);
  for (auto const& psState: psStates)
    if (psState.ps->pst == PowerSystemType::Consumer)
      res += psState.ps->capacity - psState.used;
  return res;
}

Power State::fulfilled() const& {
  Power res(0);
  for (auto const& psState: psStates)
    if (psState.ps->pst == PowerSystemType::Generator)
      res += psState.used;
  return res;
}

Power State::needFulfilled() const& {
  Power res(0);
  for (auto const& psState: psStates)
    if (psState.ps->pst == PowerSystemType::Generator)
      res += std::max(psState.keeping - psState.fulfillable(), 0_pu);
  return res;
}

Power State::keeping() const& {
  Power res(0);
  for (auto const& psState: psStates)
    res += psState.keeping;
  return res;
}

State State::demand() const& {
  State newState(*this);
  for (auto& psState: newState.psStates)
    if (psState.ps->pst == PowerSystemType::Consumer) {
      psState.keeping = psState.ps->capacity;
      psState.used    = psState.ps->capacity;
    }
  return newState;
}

std::vector<State> State::generateNextStates() {
  PLOGD << "GENERATING NEXT STATES";
  calculateOutcomes();

  std::unordered_set<State> uniqueStates;
  for (int i = 0; i < grid->tls.size(); ++i) {
    Outcome const& outcome = outcomes[grid->tls[i]->id];

    PowerSystem* inp  = grid->tls[i]->inp;
    PowerSystem* out  = outcome.gen;
    Percentage   loss = outcome.loss;
    if (psStates[inp->id].keeping == 0)
      continue;

    Power amount =
        std::min(outcome.fulfillable(psStates), psStates[inp->id].keeping);

    State nextState(*this);
    nextState.psStates[inp->id].send(amount);
    nextState.psStates[out->id].receive(amount * loss);
    nextState.psStates[out->id].fulfill();
    uniqueStates.insert(nextState);
  }

  PLOGD << "NEXT STATES";
  for (State const& nextState: uniqueStates) {
    PLOGD << nextState;
  }

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
  std::vector<Outcome*> bestOutcome(psStates.size(), nullptr);

  for (auto tl: grid->tls) {
    if (tl->out->pst != PowerSystemType::Generator)
      continue;
    if (psStates[tl->out->id].fulfillable() == 0)
      continue;
    outcomes[tl->id]         = Outcome(tl, tl->out, tl->loss);
    bestOutcome[tl->inp->id] = &outcomes[tl->id];
    q.emplace(tl->inp, bestOutcome[tl->inp->id]);
  }

  while (!q.empty()) {
    auto [ps, outcome] = q.top();
    q.pop();
    if (bestOutcome[ps->id] != outcome)
      continue;

    for (TransmissionLine* tl: ps->revAdj) {
      outcomes[tl->id] = Outcome(tl, outcome->gen, outcome->loss * tl->loss);

      if (outcomes[tl->id].fulfillable(psStates) == 0)
        continue;

      if (bestOutcome[tl->inp->id] == nullptr ||
          bestOutcome[tl->inp->id]->loss > outcomes[tl->id].loss) {
        bestOutcome[tl->inp->id] = &outcomes[tl->id];
        q.emplace(tl->inp, bestOutcome[tl->inp->id]);
      }
    }
  }
}

std::string State::toString() const& {
  if (psStates.empty())
    return "[]";

  std::stringstream ss;
  ss << "[";
  for (PowerSystemState const& psState: psStates) {
    if (psState.ps->pst != PowerSystemType::Consumer &&
        psState.ps->pst != PowerSystemType::Generator)
      continue;
    ss << fmt::format(stateElementFormat, psState.toString());
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