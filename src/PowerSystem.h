#ifndef CARRIER_H
#define CARRIER_H

#include <string>

#include "Power.h"
#include "Types.h"

enum struct PowerSystemType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct PowerSystem {
  i32             id;
  PowerSystemType pst;
  Power           capacity;

  PowerSystem(i32 id, PowerSystemType pst, Power capacity = 0);

  std::string toString() const&;
};

#endif // CARRIER_H
