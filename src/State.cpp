#include <algorithm>

#include "Carrier.h"
#include "State.h"

State::State(): depth{0} {
}

State::State(State const& o): cars{o.cars}, cbs{o.cbs}, depth{o.depth} {
}

State::State(
    std::vector<Carrier> const&        _cars,
    std::vector<CircuitBreaker> const& _cbs,
    int                                _depth
)
    : cars{_cars}, cbs{_cbs}, depth{_depth} {
}

State::State(
    std::vector<Carrier> const&&        _cars,
    std::vector<CircuitBreaker> const&& _cbs,
    int                                 _depth
)
    : cars{_cars}, cbs{_cbs}, depth{_depth} {
}

State State::demand(int idx) const& {
  State ns(*this);

  Carrier& car    = ns.cars[idx];
  int      amount = car.capacity - car.used;
  car.keeping     += amount;
  car.used        += amount;

  return ns;
}

State State::fulfill(int idx) const& {
  State ns(*this);

  Carrier& car = ns.cars[idx];
  int amount   = std::max(0_pu, std::min(car.keeping, car.capacity - car.used));
  car.used     += amount;
  car.keeping  -= amount;

  return ns;
}

std::vector<State> State::transmit(int idx) const& {
  std::vector<State> nss;
  int                inp = cbs[idx].inp;
  int                out = cbs[idx].out;
  for (Power amount{1}; amount <= cars[inp].keeping; amount += 1_pu) {
    State ns(*this);
    ns.cars[inp].keeping -= amount;
    ns.cars[out].keeping += amount.compensateLoss(cbs[idx].loss);
    nss.push_back(ns);
  }

  return nss;
}

std::string State::toString() const& {
  std::string str = "";
  for (auto const& car : cars)
    str += car.toString() + " ";
  return str;
}
