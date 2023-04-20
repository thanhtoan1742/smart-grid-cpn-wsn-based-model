#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/TransmissionLine.h>

namespace sgrid {

struct Grid {
  std::vector<PowerSystem>      pss;
  std::vector<TransmissionLine> tls;

  Grid();
  Grid(
      std::vector<PowerSystem> const&      pss,
      std::vector<TransmissionLine> const& tls
  );

  std::string toString() const&;
  std::string IdealPathToString() const&;
};

struct GridFactory {
  std::vector<PowerSystem>      pss;
  std::vector<TransmissionLine> tls;

  GridFactory&
  createPowerSystem(i32 ref, PowerSystemType pst, Power capacity = 0);
  GridFactory& createTransmissionLine(
      i32 inpRef, i32 outRef, Power capacity, Percentage loss = 0
  );
  Grid createGrid();
};

} // namespace sgrid
#endif // GRID_H
