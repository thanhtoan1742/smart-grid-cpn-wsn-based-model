#ifndef STATE_EXPLORER_H
#define STATE_EXPLORER_H

#include <memory>

#include "Grid.h"
#include "PowerSystemState.h"
#include "State.h"

namespace sgrid {

struct StateExplorer {
  Grid* grid;
  State initState;

  std::vector<std::unique_ptr<State>> stateSpace;
  State*                              bestState;
  Power                               minFulfilled;

  StateExplorer(Grid* grid, State const& initState);

  void generateStateSpace();
  void prettyPrint() const&;
  void prettyPrintBestStateTrace() const&;
};

} // namespace sgrid
#endif // STATE_EXPLORER_H
