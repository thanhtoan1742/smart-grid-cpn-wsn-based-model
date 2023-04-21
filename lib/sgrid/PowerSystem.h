#ifndef CARRIER_H
#define CARRIER_H

#include <string>
#include <vector>

#include <sgrid/Power.h>
#include <sgrid/Types.h>
#include <sgrid/TransmissionLine.h>

namespace sgrid {

enum struct PowerSystemType { Generator = 'G', Consumer = 'C', Bus = 'B' };
struct IdealPath{
  i32 nextId;
  i32 genIdx;
  Percentage loss;
  IdealPath(i32 nextId, i32 genIdx, Percentage loss);
  std::string toString() const&;
};

struct PowerSystem {
  i32             id;
  PowerSystemType pst;
  Power           capacity;

  std::vector<TransmissionLine> tls;
  std::vector<IdealPath*> idealPaths;
  i32             genIdx = -1;

  PowerSystem(i32 id, PowerSystemType pst, Power capacity = 0);

  std::string toString() const&;

  void addTransmissionLine(TransmissionLine tl);
  void updateIdealNeighbor(i32 tlIdx, i32 idx, i32 genIdx, Percentage value);
  std::vector<i32> getIdealPathOrder();
  std::string idealPathString() const&;
};

} // namespace sgrid
#endif // CARRIER_H
