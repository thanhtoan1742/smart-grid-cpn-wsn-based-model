#include "State.h"

#include <iterator>
#include <string>
#include <unordered_set>

#include "Carrier.h"
#include "CarrierState.h"
#include "Percentage.h"
#include "Power.h"
#include "utils.h"

State::State(Grid* _grid): grid(_grid), depth(0), parent(nullptr) {
  for (Carrier& car: grid->cars)
    carStates.emplace_back(&car);
}

State::State(Grid* _grid, std::vector<CarrierState> const& _carStates)
    : grid(_grid), carStates(_carStates) {
}

bool State::operator==(State const& other) const& {
  if (carStates.size() != other.carStates.size())
    return false;
  for (int i = 0; i < carStates.size(); ++i)
    if (carStates[i] != other.carStates[i])
      return false;
  return true;
}

bool State::satisfied() const& {
  for (int i = 0; i < grid->cars.size(); ++i) {
    Carrier const& car = grid->cars[i];
    if (car.ct == CarrierType::Consumer && carStates[i].used < car.capacity)
      return false;
    if (carStates[i].keeping != 0)
      return false;
  }
  return true;
}

Power State::notDemaned() const& {
  Power res(0);
  for (int i = 0; i < grid->cars.size(); ++i)
    if (grid->cars[i].ct == CarrierType::Consumer)
      res += grid->cars[i].capacity - carStates[i].used;
  return res;
}

Power State::fulfilled() const& {
  Power res(0);
  for (int i = 0; i < grid->cars.size(); ++i)
    if (grid->cars[i].ct == CarrierType::Generator)
      res += carStates[i].used;
  return res;
}

Power State::needFulfilled() const& {
  Power res(0);
  for (int i = 0; i < grid->cars.size(); ++i)
    if (grid->cars[i].ct == CarrierType::Generator) {
      Power need = grid->cars[i].capacity - carStates[i].used;
      res        += std::max(0_pu, carStates[i].keeping - need);
    }
  return res;
}

Power State::keeping() const& {
  Power res(0);
  for (auto const& carState: carStates)
    res += carState.keeping;
  return res;
}

State State::createChildState(int idx, CarrierState const& newCarState) const& {
  std::vector<CarrierState> newCarStates(carStates);
  newCarStates[idx] = std::move(newCarState);
  return State(grid, newCarStates);
}

std::vector<State> State::generateNextStates() const& {
  std::unordered_set<State> uniqueStates;
  for (int i = 0; i < grid->cars.size(); ++i) {
    if (grid->cars[i].ct == CarrierType::Generator) {
      uniqueStates.insert(createChildState(i, carStates[i].fulfill()));
    }
    if (grid->cars[i].ct == CarrierType::Consumer) {
      uniqueStates.insert(createChildState(i, carStates[i].demand()));
    }
  }

  for (int i = 0; i < grid->cbs.size(); ++i) {
    i32        inp  = grid->cbs[i].inp;
    i32        out  = grid->cbs[i].out;
    Percentage loss = grid->cbs[i].loss;

    Power maxAmount = std::min(carStates[inp].keeping, grid->cbs[i].capacity);
    for (Power amount = maxAmount; amount > 0_pu; amount -= 1) {
      State nextState(*this);
      nextState.carStates[inp] = nextState.carStates[inp].send(amount);
      nextState.carStates[out] =
          nextState.carStates[out].receive(amount.compensateLoss(loss));
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
  if (carStates.empty())
    return "[]";
  std::string str = "[";
  for (auto const& carState: carStates)
    str += padded(
        std::to_string(carState.car->id) + "|" + carState.toString() + " "
    );
  str[str.size() - 1] = ']';
  return str;
}

std::size_t std::hash<State>::operator()(State const& state) const noexcept {
  std::string str;
  for (auto const& carState: state.carStates)
    str += std::to_string(carState.keeping) + ":" +
           std::to_string(carState.used) + " ";
  return std::hash<std::string>()(str);
}