#ifndef SGRID_STATE_EXPLORER_H
#define SGRID_STATE_EXPLORER_H

#include <sgrid/Grid.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/State.h>

#include <memory>

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
#endif // SGRID_STATE_EXPLORER_H
