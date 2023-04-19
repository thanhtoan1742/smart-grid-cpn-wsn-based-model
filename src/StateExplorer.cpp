#include "StateExplorer.h"

#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <unordered_set>

#include "State.h"
#include "Types.h"
#include "utils.h"

namespace sgrid {

StateExplorer::StateExplorer(Grid* _grid, State const& _initState)
    : grid(_grid), initState(_initState) {
}

struct CompareState {
  // return true means priorize b, false means prioritize a.
  bool operator()(State const* a, State const* b) {

    // debug("COMPARING", a->toString(), " and ", b->toString());
    Power notDemandedA = a->notDemaned();
    Power notDemamdedB = b->notDemaned();
    if (notDemandedA != notDemamdedB) {
      // debug("Demanded:", notDemandedA, " and ", notDemamdedB);
      return notDemandedA > notDemamdedB;
    }

    Power keepingA = a->keeping() + a->fulfilled();
    Power keepingB = b->keeping() + b->fulfilled();
    if (keepingA != keepingB)
      return keepingA > keepingB;

    Power fulfilledA = a->needFulfilled();
    Power fulfilledB = b->needFulfilled();
    if (fulfilledA != fulfilledB) {
      // debug("Fulfilled:", fulfilledA, " and ", fulfilledB);
      return fulfilledA < fulfilledB;
    }

    return a->depth > b->depth;
  }
};

void StateExplorer::generateStateSpace() {
  bestState = nullptr;
  std::priority_queue<State*, std::vector<State*>, CompareState> q;
  std::unordered_set<State>                                      visited;

  stateSpace.push_back(std::make_unique<State>(initState));
  q.push(stateSpace.back().get());
  visited.insert(initState);
  minFulfilled = Power(std::numeric_limits<u64>::max());

  while (!q.empty()) {
    State* currentState = q.top();
    q.pop();
    // debug(
    //     "PROCESSING STATE",
    //     "\n",
    //     !currentState->parent ? "[]" : currentState->parent->toString(),
    //     "\n",
    //     currentState->toString(),
    //     "\n",
    //     currentState->keeping() + currentState->fulfilled()
    // );

    if (currentState->keeping() + currentState->fulfilled() >= minFulfilled)
      continue;
    // if (currentState->depth == 4){
    //   break;
    // }
    if (currentState->satisfied()) {
      // debug("SATISFIED STATE", currentState->toString());
      minFulfilled = currentState->fulfilled();
      bestState    = currentState;
      break;
    }
    // debug("NEXT STATES SIZE", currentState->generateNextStates().size());

    for (State& nextState: currentState->generateNextStates()) {
      auto [_, inserted] = visited.insert(nextState);
      if (!inserted)
        continue;

      stateSpace.push_back(std::make_unique<State>(nextState));
      State* nextStatePtr = stateSpace.back().get();
      q.push(nextStatePtr);
      currentState->children.push_back(nextStatePtr);
      nextStatePtr->parent = currentState;
      nextStatePtr->depth  = currentState->depth + 1;
      // if (currentState->toString() == "[0:10 0:10 0:0 10:0 0:0 13:0 0:0
      // 0:0]") debug("GEN STATE", currentState->toString(), " --> ",
      // nextStatePtr->toString(), " | ", nextStatePtr->keeping() +
      // nextStatePtr->fulfilled());
    }
  }
}

void prettyPrintState(State const& state) {
  for (i32 i = 0; i < state.depth; ++i)
    std::cout << " ";
  std::cout << state.toString() << " ";
  std::cout << std::endl;
}

void prettyPrintRec(State const& state) {
  prettyPrintState(state);
  for (State* child: state.children)
    prettyPrintRec(*child);
}

void StateExplorer::prettyPrint() const& {
  if (stateSpace.empty())
    return;
  prettyPrintRec(*stateSpace[0]);
}

void StateExplorer::prettyPrintBestStateTrace() const& {
  State* current = bestState;
  while (current != nullptr) {
    prettyPrintState(*current);
    current = current->parent;
  }
}

} // namespace sgrid
