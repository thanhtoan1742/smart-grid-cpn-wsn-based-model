#ifndef CARRIER_H
#define CARRIER_H

#include <string>

#include "Power.h"
#include "Types.h"

enum struct CarrierType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct Carrier {
  i32         id;
  CarrierType ct;
  Power       capacity;

  Carrier(i32 id, CarrierType ct, Power capacity = 0);

  std::string toString() const&;
};

#endif // CARRIER_H
