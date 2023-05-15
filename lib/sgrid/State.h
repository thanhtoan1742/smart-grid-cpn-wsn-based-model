#ifndef SGRID_STATE_H
#define SGRID_STATE_H

#include <sgrid/Grid.h>
#include <sgrid/Outcome.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/TransmissionLineState.h>

#include <plog/Log.h>
#include <plog/Record.h>

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>

namespace sgrid {

struct State {
  Grid*                               grid;
  std::vector<PowerSystemState*>      psStates;
  std::vector<TransmissionLineState*> tlStates;
  std::vector<Outcome*>               outcomes;
  std::vector<Outcome*>               bestOutcome;

  i32                 depth;
  State*              parent;
  std::vector<State*> children;

  State(Grid* grid);
  State(State const& original);
  ~State();

  bool operator==(State const& other) const&;

  bool  satisfied() const&;
  Power notDemaned() const&;
  Power fulfilled() const&;
  Power needFulfilled() const&;
  Power keeping() const&;

  void               demand();
  std::vector<State> generateNextStates();

  void calculateOutcomes();

  std::string toString() const&;
};

} // namespace sgrid

template <>
struct std::hash<sgrid::State> {
  size_t operator()(sgrid::State const& state) const noexcept;
};

namespace plog {
Record& operator<<(Record& record, sgrid::State const& state);
} // namespace plog

#endif // SGRID_STATE_H
