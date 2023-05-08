#ifndef SGRID_CARRIER_H
#define SGRID_CARRIER_H

#include <sgrid/Power.h>
#include <sgrid/Types.h>

#include <string>
#include <vector>

namespace sgrid {

struct TransmissionLine;

enum struct PowerSystemType { Generator = 'G', Consumer = 'C', Bus = 'B' };

struct PowerSystem {
  i32             id;
  PowerSystemType pst;
  Power           capacity;

  std::vector<TransmissionLine*> revAdj;

  PowerSystem(i32 id, PowerSystemType pst, Power capacity = 0);

  std::string toString() const&;
};

} // namespace sgrid
#endif // SGRID_CARRIER_H
