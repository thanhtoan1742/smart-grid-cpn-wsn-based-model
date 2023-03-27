#include "State.h"
#include "Carrier.h"

int withLoss(int amount, int loss) {
  // floor(amount * (100 + loss)/100)
  return (amount * (100 + loss) + 99) / 100;
}

State::State() : depth(0) {}
State::State(std::vector<Carrier> const &_cars,
             std::vector<CircuitBreaker> const &_cbs, int _depth)
    : cars(_cars), cbs(_cbs), depth(_depth) {}
State::State(State const &original)
    : cars(original.cars), cbs(original.cbs), depth(original.depth) {}

State State::demand(int idx) const & {
  State ns(*this);
  Carrier &car = ns.cars[idx];
  int amount = car.capacity - car.used;
  car.keeping += amount;
  car.used += amount;
  return ns;
}

State State::fulfill(int idx) const & {
  State ns(*this);
  Carrier &car = ns.cars[idx];
  int amount = std::max(0, std::min(car.keeping, car.capacity - car.used));
  car.used += amount;
  car.keeping -= amount;
  return ns;
}

std::vector<State> State::transmit(int idx) const & {
  std::vector<State> nss;
  int inp = cbs[idx].inp;
  int out = cbs[idx].out;
  for (int amount = 1; amount <= cars[inp].keeping; ++amount) {
    State ns(*this);
    ns.cars[inp].keeping -= amount;
    int amountWithLoss = withLoss(amount, cbs[idx].loss);
    ns.cars[out].keeping += amountWithLoss;
    nss.push_back(ns);
  }

  return nss;
}

std::vector<State> State::generateNextStates() const & {
  std::vector<State> nss;
  for (int i = 0; i < cars.size(); ++i) {
    if (cars[i].ct == CarrierType::Consumer)
      nss.push_back(demand(i));
    if (cars[i].ct == CarrierType::Generator)
      nss.push_back(fulfill(i));
  }

  for (int i = 0; i < cbs.size(); ++i) {
    auto transmitNss = transmit(i);
    for (auto const &s : transmitNss)
      nss.push_back(s);
  }

  return nss;
}

std::string State::toString() const & {
  std::string str = "";
  for (auto const &car : cars)
    str += car.toString() + " ";
  return str;
}
