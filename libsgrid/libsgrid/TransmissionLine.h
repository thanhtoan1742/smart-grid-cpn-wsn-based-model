#ifndef CIRCUIT_BREAKER_H
#define CIRCUIT_BREAKER_H

#include <string>

#include <libsgrid/Percentage.h>
#include <libsgrid/Power.h>
#include <libsgrid/Types.h>

namespace sgrid {

struct TransmissionLine {
  i32        id;
  i32        inp;
  i32        out;
  Power      capacity;
  Percentage loss;

  TransmissionLine(
      i32 id, i32 inp, i32 out, Power capacity = 0, Percentage loss = 0
  );

  std::string toString() const&;
};

} // namespace sgrid
#endif // CIRCUIT_BREAKER_H
