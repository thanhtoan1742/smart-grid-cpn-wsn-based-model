#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>

#include "Carrier.h"
#include "CircuitBreaker.h"

struct State {
  std::vector<Carrier>        cars;
  std::vector<CircuitBreaker> cbs;
  int                         depth;

  State();
  State(
      std::vector<Carrier> const&        cars,
      std::vector<CircuitBreaker> const& cbs,
      int                                depth = 0
  );
  State(State const& original);
  State(
      std::vector<Carrier> const&&        cars,
      std::vector<CircuitBreaker> const&& cbs,
      int                                 depth = 0
  );

  State              demand(int idx) const&;
  State              fulfill(int idx) const&;
  std::vector<State> transmit(int idx) const&;
  std::string        toString() const&;
};

#endif // STATE_H
