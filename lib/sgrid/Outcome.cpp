#include <sgrid/Outcome.h>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/TransmissionLine.h>

#include <fmt/core.h>

#include <string>
#include <vector>

namespace sgrid {

Outcome::Outcome(
    TransmissionLine* _tl,
    PowerSystem*      _gen,
    Percentage        _loss,
    TransmissionLine* _capTl,
    Percentage        _capTlLoss
)
    : tl(_tl), gen(_gen), loss(_loss), capTl(_capTl), capTlLoss(_capTlLoss) {
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