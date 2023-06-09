#ifndef SGRID_GRID_H
#define SGRID_GRID_H

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/TransmissionLine.h>

#include <map>
#include <string>
#include <vector>

namespace sgrid {

struct Grid {
  std::vector<PowerSystem*>      pss;
  std::vector<TransmissionLine*> tls;

  Grid();
  Grid(Grid const& original);
  Grid(
      std::vector<PowerSystem*> const&      pss,
      std::vector<TransmissionLine*> const& tls
  );
  ~Grid();

  std::string toString() const&;
};

struct GridFactory {
  std::vector<PowerSystem*>      pss;
  std::vector<TransmissionLine*> tls;
  std::map<i32, PowerSystem*>    ref2ps;

  GridFactory&
  createPowerSystem(i32 ref, PowerSystemType pst, Power capacity = 0);
  GridFactory& createTransmissionLine(
      i32 inpRef, i32 outRef, Power capacity, Percentage loss = 0
  );
  Grid createGrid();
};

} // namespace sgrid
#endif // SGRID_GRID_H
