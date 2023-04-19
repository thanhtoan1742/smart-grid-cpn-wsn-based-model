#ifndef STATE_H
#define STATE_H

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>

#include "Grid.h"
#include "PowerSystemState.h"

namespace sgrid {

struct State {
  Grid*                         grid;
  std::vector<PowerSystemState> psStates;

  i32                 depth;
  State*              parent;
  std::vector<State*> children;

  State(Grid* grid);
  State(Grid* grid, std::vector<PowerSystemState> const& psStates);

  bool operator==(State const& other) const&;

  bool  satisfied() const&;
  Power notDemaned() const&;
  Power fulfilled() const&;
  Power needFulfilled() const&;
  Power keeping() const&;

  State createChildState(i32 idx, PowerSystemState const& psState) const&;
  std::vector<State> generateNextStates() const&;

  std::string toString() const&;
};

} // namespace sgrid

template <>
struct std::hash<sgrid::State> {
  size_t operator()(sgrid::State const& state) const noexcept;
};

#endif // STATE_H
