#ifndef CARRIER_H
#define CARRIER_H

#include <string>

#include <lib/sgrid/Power.h>
#include <lib/sgrid/Types.h>

namespace sgrid {

enum struct PowerSystemType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct PowerSystem {
  i32             id;
  PowerSystemType pst;
  Power           capacity;

  PowerSystem(i32 id, PowerSystemType pst, Power capacity = 0);

  std::string toString() const&;
};

} // namespace sgrid
#endif // CARRIER_H
