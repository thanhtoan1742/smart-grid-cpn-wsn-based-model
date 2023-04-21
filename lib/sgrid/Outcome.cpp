#include <sgrid/Outcome.h>

#include <fmt/core.h>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/TransmissionLine.h>
#include <string>

namespace sgrid {

Outcome::Outcome(Percentage _loss, PowerSystem* _gen, TransmissionLine* _tl)
    : loss(_loss), gen(_gen), tl(_tl) {
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