#ifndef STATE_H
#define STATE_H

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>

#include <sgrid/Grid.h>
#include <sgrid/Outcome.h>
#include <sgrid/PowerSystemState.h>

namespace sgrid {

struct State {
  Grid*                         grid;
  std::vector<PowerSystemState> psStates;

  std::vector<Outcome> outcomes;

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
  State demand() const&;
  std::vector<State> generateNextStates() const&;

  void calculateOutcomes();

  std::string toString() const&;
};

} // namespace sgrid

template <>
struct std::hash<sgrid::State> {
  size_t operator()(sgrid::State const& state) const noexcept;
};

#endif // STATE_H
