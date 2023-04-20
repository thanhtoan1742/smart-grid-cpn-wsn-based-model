#ifndef STATE_EXPLORER_H
#define STATE_EXPLORER_H

#include <memory>

#include <libsgrid/Grid.h>
#include <libsgrid/PowerSystemState.h>
#include <libsgrid/State.h>

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
