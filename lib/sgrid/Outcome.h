#ifndef SGRID_OUTCOME_H
#define SGRID_OUTCOME_H

#include "sgrid/TransmissionLineState.h"
#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/Types.h>

namespace sgrid {

struct State;

struct Outcome {
  State*            state;
  TransmissionLine* tl;

  PowerSystem*      gen;
  Percentage        loss;
  TransmissionLine* congestedTl;
  Percentage        congestedTlLoss;

  Outcome* trace;

  Outcome(
      State*            state,
      TransmissionLine* tl,
      PowerSystem*      gen,
      Percentage        loss,
      TransmissionLine* congestedTl,
      Percentage        congestedTlLoss,
      Outcome*          trace = nullptr
  );

  Power fulfillable(
      PowerSystemState* genState, TransmissionLineState* congestedTlState
  ) const&;
  Power fulfillable() const&;

  std::string toString() const&;
};

} // namespace sgrid
#endif // SGRID_OUTCOME_H