#ifndef STATE_H
#define STATE_H

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>

#include "CarrierState.h"
#include "Grid.h"

struct State {
  Grid*                     grid;
  std::vector<CarrierState> carStates;

  i32                 depth;
  State*              parent;
  std::vector<State*> children;

  State(Grid* grid);
  State(Grid* grid, std::vector<CarrierState> const& carState);

  bool operator==(State const& other) const&;

  bool  satisfied() const&;
  Power notDemaned() const&;
  Power fulfilled() const&;
  Power need_fulfilled() const&;
  Power keeping() const&;

  State createChildState(int idx, CarrierState const& carState) const&;
  std::vector<State> generateNextStates() const&;

  std::string toString() const&;
};

template <>
struct std::hash<State> {
  size_t operator()(State const& state) const noexcept;
};

#endif // STATE_H
