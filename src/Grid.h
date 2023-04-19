#ifndef GRID_H
#define GRID_H

#include <string>
#include <vector>

#include "PowerSystem.h"
#include "TransmissionLine.h"

struct Grid {
  std::vector<PowerSystem>      cars;
  std::vector<TransmissionLine> cbs;

  Grid();
  Grid(
      std::vector<PowerSystem> const&      cars,
      std::vector<TransmissionLine> const& cbs
  );

  std::string toString() const&;
};

struct TransmissionLineVectorFactory {
  std::vector<TransmissionLine> cbs;

  TransmissionLineVectorFactory& addTransmissionLine(
      int inp, int out, Power capacity = 0, Percentage loss = 0
  );
  std::vector<TransmissionLine> toVector();
};

#endif // GRID_H
