#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

#include "Percentage.h"
#include "PowerSystem.h"
#include "TransmissionLine.h"

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
};

struct GridFactory {
  std::vector<PowerSystem>      pss;
  std::vector<TransmissionLine> tls;

  GridFactory& createPowerSystem(PowerSystemType pst, Power capacity = 0);
  GridFactory&
  createTransmissionLine(i32 inp, i32 out, Power capacity, Percentage loss = 0);
  Grid createGrid();
};

} // namespace sgrid
#endif // GRID_H
