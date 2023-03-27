#ifndef CARRIER_H
#define CARRIER_H

#include <string>

enum struct CarrierType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct Carrier {
  int const         id;
  CarrierType const ct;
  int const         capacity;
  int               keeping;
  int               used;

  Carrier(
      int id, CarrierType ct, int capacity = 0, int keeping = 0, int used = 0
  );
  std::string toString() const&;
};

#endif // CARRIER_H
