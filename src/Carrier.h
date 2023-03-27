#ifndef CARRIER_H
#define CARRIER_H

#include <string>

enum struct CarrierType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct Carrier {
  const int id;
  const CarrierType ct;
  const int capacity;
  int keeping;
  int used;

  Carrier(int id, CarrierType ct, int capacity = 0, int keeping = 0,
          int used = 0);
  std::string toString() const &;
};

#endif // CARRIER_H
