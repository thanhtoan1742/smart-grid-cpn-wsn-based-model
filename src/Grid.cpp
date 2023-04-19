#include "Grid.h"
#include "CircuitBreaker.h"
#include "Percentage.h"
#include "utils.h"
#include <vector>

Grid::Grid() {
}

Grid::Grid(
    std::vector<Carrier> const& _cars, std::vector<CircuitBreaker> const& _cbs
)
    : cars{_cars}, cbs{_cbs} {
}

std::string Grid::toString() const& {
  std::string str = "";
  for (auto const& car: cars)
    str += car.toString() + ", ";
  str[str.size() - 1] = '\n';
  for (auto const& cb: cbs)
    str += cb.toString() + "\n";
  return str;
}

CircuitBreakerVectorFactory& CircuitBreakerVectorFactory::addCircuitBreaker(
    int inp, int out, Power capacity, Percentage loss
) {
  cbs.emplace_back(0, inp, out, capacity, loss);
  return *this;
}

std::vector<CircuitBreaker> CircuitBreakerVectorFactory::toVector() {
  int sz = cbs.size();
  for (int i = 0; i < sz; ++i)
    cbs.emplace_back(0, cbs[i].out, cbs[i].inp, cbs[i].capacity, cbs[i].loss);
  for (int i = 0; i < cbs.size(); ++i) {
    cbs[i].id = i;
  }
  return std::move(cbs);
}