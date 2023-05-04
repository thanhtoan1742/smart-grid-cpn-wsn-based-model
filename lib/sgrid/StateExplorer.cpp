#include <sgrid/StateExplorer.h>

#include <sgrid/State.h>
#include <sgrid/Types.h>
#include <sgrid/utils.h>

#include <iostream>
#include <limits>
#include <memory>
#include <numeric>
#include <queue>
#include <sstream>
#include <unordered_set>

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

  initState = initState.demand();
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

    if (currentState->satisfied()) {
      // debug("SATISFIED STATE", currentState->toString());
      minFulfilled = currentState->fulfilled();
      bestState    = currentState;
      break;
    }

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
    }
  }
}

void stateToPaddedString(std::stringstream& ss, State const& state) {
  for (i32 i = 0; i < state.depth; ++i)
    ss << " ";
  ss << state.toString() << " ";
  ss << std::endl;
}

void toPrettyStringRec(std::stringstream& ss, State const& state) {
  stateToPaddedString(ss, state);
  for (State* child: state.children)
    toPrettyStringRec(ss, *child);
}

std::string StateExplorer::stateSpaceToString() const& {
  if (stateSpace.empty())
    return "";

  std::stringstream ss;
  toPrettyStringRec(ss, *stateSpace[0]);
  return ss.str();
}

std::string StateExplorer::bestStateTraceToString() const& {
  std::stringstream ss;
  State*            current = bestState;
  while (current != nullptr) {
    stateToPaddedString(ss, *current);
    current = current->parent;
  }
  return ss.str();
}

} // namespace sgrid
