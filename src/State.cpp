#include "State.h"

#include <iterator>
#include <string>
#include <unordered_set>

#include "Percentage.h"
#include "Power.h"
#include "PowerSystem.h"
#include "PowerSystemState.h"
#include "utils.h"

namespace sgrid {

State::State(Grid* _grid): grid(_grid), depth(0), parent(nullptr) {
  for (PowerSystem& car: grid->pss)
    psStates.emplace_back(&car);
}

State::State(Grid* _grid, std::vector<PowerSystemState> const& _psStates)
    : grid(_grid), psStates(_psStates) {
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

std::vector<State> State::generateNextStates() const& {
  std::unordered_set<State> uniqueStates;
  for (int i = 0; i < grid->pss.size(); ++i) {
    if (grid->pss[i].pst == PowerSystemType::Generator) {
      uniqueStates.insert(createChildState(i, psStates[i].fulfill()));
    }
    if (grid->pss[i].pst == PowerSystemType::Consumer) {
      uniqueStates.insert(createChildState(i, psStates[i].demand()));
    }
  }

  for (int i = 0; i < grid->tls.size(); ++i) {
    i32        inp  = grid->tls[i].inp;
    i32        out  = grid->tls[i].out;
    Percentage loss = grid->tls[i].loss;

    Power maxAmount = std::min(psStates[inp].keeping, grid->tls[i].capacity);
    for (Power amount = maxAmount; amount > 0_pu; amount -= 1) {
      State nextState(*this);
      nextState.psStates[inp] = nextState.psStates[inp].send(amount);
      nextState.psStates[out] =
          nextState.psStates[out].receive(amount.compensateLoss(loss));
      uniqueStates.insert(nextState);
      break; // only try maxAmount
    }
  }

  return std::vector<State>(
      std::make_move_iterator(uniqueStates.begin()),
      std::make_move_iterator(uniqueStates.end())
  );
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