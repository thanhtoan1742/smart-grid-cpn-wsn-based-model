#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

#include "Carrier.h"
#include "CircuitBreaker.h"

struct Grid {
  std::vector<Carrier>        cars;
  std::vector<CircuitBreaker> cbs;

  Grid();
  Grid(
      std::vector<Carrier> const& cars, std::vector<CircuitBreaker> const& cbs
  );

  std::string toString() const&;
};

struct CircuitBreakerVectorFactory {
  std::vector<CircuitBreaker> cbs;

  CircuitBreakerVectorFactory&
  addCircuitBreaker(int inp, int out, Power capacity = 0, Percentage loss = 0);
  std::vector<CircuitBreaker> toVector();
};

#endif // GRID_H
