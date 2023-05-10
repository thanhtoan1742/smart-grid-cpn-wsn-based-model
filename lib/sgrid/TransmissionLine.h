#ifndef SGRID_CIRCUIT_BREAKER_H
#define SGRID_CIRCUIT_BREAKER_H

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/types.h>

#include <string>

namespace sgrid {

struct PowerSystem;

struct TransmissionLine {
  i32          id;
  PowerSystem* inp;
  PowerSystem* out;
  Power        capacity;
  Percentage   loss;

  TransmissionLine(
      i32          id,
      PowerSystem* inp,
      PowerSystem* out,
      Power        capacity = 0,
      Percentage   loss     = 0
  );

  std::string toString() const&;
};

} // namespace sgrid
#endif // SGRID_CIRCUIT_BREAKER_H
