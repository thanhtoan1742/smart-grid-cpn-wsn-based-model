#include "sgrid/Outcome.h"
#include "sgrid/TransmissionLine.h"
#include <sgrid/State.h>

#include <iterator>
#include <limits.h>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/utils.h>

namespace sgrid {

State::State(Grid* _grid)
    : grid(_grid), depth(0), parent(nullptr), outcomes(_grid->tls.size()) {
  for (PowerSystem& car: grid->pss)
    psStates.emplace_back(&car);
}

State::State(Grid* _grid, std::vector<PowerSystemState> const& _psStates)
    : grid(_grid), psStates(_psStates), outcomes(_grid->tls.size()) {
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
    PowerSystem const& ps = grid->pss[i];
    if (ps.pst == PowerSystemType::Consumer && psStates[i].used < ps.capacity)
      return false;
    if (psStates[i].keeping != 0)
      return false;
  }
  return true;
}

Power State::notDemaned() const& {
  Power res(0);
  for (int i = 0; i < grid->pss.size(); ++i)
    if (grid->pss[i].pst == PowerSystemType::Consumer)
      res += grid->pss[i].capacity - psStates[i].used;
  return res;
}

Power State::fulfilled() const& {
  Power res(0);
  for (int i = 0; i < grid->pss.size(); ++i)
    if (grid->pss[i].pst == PowerSystemType::Generator)
      res += psStates[i].used;
  return res;
}

Power State::needFulfilled() const& {
  Power res(0);
  for (int i = 0; i < grid->pss.size(); ++i)
    if (grid->pss[i].pst == PowerSystemType::Generator) {
      Power need = grid->pss[i].capacity - psStates[i].used;
      res        += std::max(0_pu, psStates[i].keeping - need);
    }
  return res;
}

Power State::keeping() const& {
  Power res(0);
  for (auto const& psState: psStates)
    res += psState.keeping;
  return res;
}

State State::createChildState(i32 idx, PowerSystemState const& newPsState)
    const& {
  std::vector<PowerSystemState> newPsStates(psStates);
  newPsStates[idx] = std::move(newPsState);
  return State(grid, newPsStates);
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
  calculateOutcomes();

  std::unordered_set<State> uniqueStates;
  for (int i = 0; i < grid->pss.size(); ++i) {
    if (grid->pss[i].pst == PowerSystemType::Generator)
      uniqueStates.insert(createChildState(i, psStates[i].fulfill()));
  }

  for (int i = 0; i < grid->tls.size(); ++i) {
    i32        inp  = grid->tls[i].inp;
    i32        out  = grid->tls[i].out;
    Percentage loss = grid->tls[i].loss;

    Outcome const& outcome = outcomes[grid->tls[i].id];
    Power          amount  = std::min(
        psStates[outcome.gen->id].fulfillable() / outcome.loss,
        psStates[inp].keeping
    );

    State nextState(*this);
    nextState.psStates[inp] = nextState.psStates[inp].send(amount);
    nextState.psStates[out] = nextState.psStates[out].receive(amount * loss);
    uniqueStates.insert(nextState);
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

  for (TransmissionLine& tl: grid->tls) {
    PowerSystem& gen = grid->pss[tl.out];
    PowerSystem& inp = grid->pss[tl.inp];
    if (gen.pst != PowerSystemType::Generator)
      continue;
    outcomes[tl.id]     = Outcome(tl.loss, &gen, &tl);
    bestOutcome[inp.id] = &outcomes[tl.id];
    q.emplace(&inp, bestOutcome[inp.id]);
  }

  while (!q.empty()) {
    auto [ps, outcome] = q.top();
    q.pop();
    if (bestOutcome[ps->id] != outcome)
      continue;

    for (TransmissionLine* tl: ps->revAdj) {
      PowerSystem& inp = grid->pss[tl->inp];
      outcomes[tl->id] = Outcome(outcome->loss * tl->loss, outcome->gen, tl);

      if (bestOutcome[inp.id] == nullptr ||
          bestOutcome[inp.id]->loss > outcomes[tl->id].loss) {
        bestOutcome[inp.id] = &outcomes[tl->id];
        q.emplace(&inp, bestOutcome[inp.id]);
      }
    }
  }
}

std::string State::toString() const& {
  if (psStates.empty())
    return "[]";
  std::string str = "[";
  for (auto const& psState: psStates)
    str +=
        padded(std::to_string(psState.ps->id) + "|" + psState.toString() + " ");
  str[str.size() - 1] = ']';
  return str;
}

} // namespace sgrid

std::size_t std::hash<sgrid::State>::operator()(sgrid::State const& state
) const noexcept {
  std::string str;
  for (auto const& psState: state.psStates)
    str += std::to_string(psState.keeping) + ":" +
           std::to_string(psState.used) + " ";
  return std::hash<std::string>()(str);
}