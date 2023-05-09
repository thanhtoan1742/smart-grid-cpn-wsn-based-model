#include <sgrid/Outcome.h>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/State.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/TransmissionLineState.h>

#include <fmt/core.h>
#include <fmt/format.h>
#include <plog/Log.h>

#include <string>
#include <vector>

namespace sgrid {

Outcome::Outcome(
    State*            _state,
    TransmissionLine* _tl,
    PowerSystem*      _gen,
    Percentage        _loss,
    TransmissionLine* _congestedTl,
    Percentage        _congestedTlLoss,
    Outcome*          _trace
)
    : state(_state),
      tl(_tl),
      gen(_gen),
      loss(_loss),
      congestedTl(_congestedTl),
      congestedTlLoss(_congestedTlLoss),
      trace(_trace) {
}

Power Outcome::fulfillable(
    PowerSystemState* genState, TransmissionLineState* congestedTlState
) const& {
  return std::min(
      genState->fulfillable() / loss,
      congestedTlState->transmittable() / congestedTlLoss
  );
}

Power Outcome::fulfillable() const& {
  return fulfillable(
      state->psStates[gen->id],
      state->tlStates[congestedTl->id]
  );
}

std::string Outcome::toString() const& {
  return fmt::format(
      "tl:{}, gen:{}, loss:{}, congestedTl:{}, congestedTlLoss: {}",
      tl->toString(),
      gen->toString(),
      loss.toString(),
      congestedTl->toString(),
      congestedTlLoss.toString()
  );
}

} // namespace sgrid