#include "sgrid/PowerSystemState.h"
#include <sgrid/Outcome.h>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/TransmissionLine.h>

#include <fmt/core.h>

#include <string>
#include <vector>

namespace sgrid {

Outcome::Outcome(Percentage _loss, PowerSystem* _gen, TransmissionLine* _tl)
    : loss(_loss), gen(_gen), tl(_tl) {
}

Power Outcome::fulfillable(PowerSystemState const& genState) const& {
  return genState.fulfillable() / loss;
}

Power Outcome::fulfillable(std::vector<PowerSystemState> const& psStates
) const& {
  return fulfillable(psStates[gen->id]);
}

std::string Outcome::toString() const& {
  return fmt::format(
      "tl:{}, gen:{}, loss:{}",
      tl->toString(),
      gen->toString(),
      loss.toString()
  );
}

} // namespace sgrid