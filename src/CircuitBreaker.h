#ifndef CIRCUIT_BREAKER_H
#define CIRCUIT_BREAKER_H

#include "Percentage.h"
#include "Power.h"

struct CircuitBreaker {
  int const        id;
  int const        inp;
  int const        out;
  Power const      capacity;
  Percentage const loss;

  CircuitBreaker(
      int id, int inp, int out, Power capacity = 0, Percentage loss = 0
  );
};

#endif // CIRCUIT_BREAKER_H
