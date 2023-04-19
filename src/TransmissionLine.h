#ifndef CIRCUIT_BREAKER_H
#define CIRCUIT_BREAKER_H

#include <string>

#include "Percentage.h"
#include "Power.h"
#include "Types.h"

struct TransmissionLine {
  i32        id;
  i32        inp;
  i32        out;
  Power      capacity;
  Percentage loss;

  TransmissionLine(
      int id, int inp, int out, Power capacity = 0, Percentage loss = 0
  );

  std::string toString() const&;
};

#endif // CIRCUIT_BREAKER_H
