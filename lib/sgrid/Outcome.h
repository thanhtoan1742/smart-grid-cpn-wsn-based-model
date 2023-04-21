#ifndef OUTCOME_H
#define OUTCOME_H

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
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

  std::string toString() const&;
};

} // namespace sgrid
#endif // OUTCOME_H