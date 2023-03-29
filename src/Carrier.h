#ifndef CARRIER_H
#define CARRIER_H

#include <string>

#include "Power.h"

enum struct CarrierType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct Carrier {
  int const         id;
  CarrierType const ct;
  Power const       capacity;
  Power             keeping;
  Power             used;

  Carrier(
      int         id,
      CarrierType ct,
      Power       capacity = 0,
      Power       keeping  = 0,
      Power       used     = 0
  );
  std::string toString() const&;
};

#endif // CARRIER_H
