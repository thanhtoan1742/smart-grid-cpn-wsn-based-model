#ifndef SGRID_OUTCOME_H
#define SGRID_OUTCOME_H

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/Types.h>

namespace sgrid {

struct Outcome {
  TransmissionLine* tl;
  PowerSystem*      gen;
  Percentage        loss;
  TransmissionLine* capTl;
  Percentage        capTlLoss;

  Outcome(
      TransmissionLine* tl   = nullptr,
      PowerSystem*      gen  = nullptr,
      Percentage        loss = 0,
      // transmission line that have capacity limit the transmission amount
      TransmissionLine* capTl     = nullptr,
      Percentage        capTlLoss = 0
  );

  Power fulfillable(PowerSystemState const& genState) const&;
  Power fulfillable(std::vector<PowerSystemState> const& psStates) const&;

  std::string toString() const&;
};

} // namespace sgrid
#endif // SGRID_OUTCOME_H