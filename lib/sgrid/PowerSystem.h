#ifndef CARRIER_H
#define CARRIER_H

#include <string>
#include <vector>

#include <sgrid/Power.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/Types.h>

namespace sgrid {

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
#endif // CARRIER_H
