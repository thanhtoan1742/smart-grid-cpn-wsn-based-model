#include "Grid.h"

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
  for (auto const& cb: cbs)
    str += cb.toString() + "\n";
  return str;
}
