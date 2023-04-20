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
  int nextId;
  i32 genIdx;
  Percentage loss;
  IdealPath(int nextId, i32 genIdx, Percentage loss);
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

  void addTransmissionLine(TransmissionLine tl);
  void updateIdealNeighbor(int tlIdx, int idx, i32 genIdx, Percentage value);
  std::vector<int> getIdealPathOrder();
  std::string toString() const&;
  std::string IdealPathToString() const&;
};

} // namespace sgrid
#endif // CARRIER_H
