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
  Percentage        loss;
  PowerSystem*      gen;
  TransmissionLine* tl;

  Outcome(
      Percentage        loss = 0,
      PowerSystem*      gen  = nullptr,
      TransmissionLine* tl   = nullptr
  );

  Power fulfillable(PowerSystemState const& genState) const&;
  Power fulfillable(std::vector<PowerSystemState> const& psStates) const&;

  std::string toString() const&;
};

} // namespace sgrid
#endif // SGRID_OUTCOME_H