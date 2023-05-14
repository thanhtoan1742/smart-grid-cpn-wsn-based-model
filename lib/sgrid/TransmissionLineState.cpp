#include <sgrid/TransmissionLine.h>

#include <sgrid/Power.h>
#include <sgrid/TransmissionLineState.h>

#include <fmt/core.h>

#include <string>

namespace sgrid {
TransmissionLineState::TransmissionLineState(TransmissionLine* _tl, Power _used)
    : tl(_tl), used(_used) {
}

bool TransmissionLineState::operator==(TransmissionLineState const& other
) const& {
  // TODO: also compare tl == other.tl
  return used == other.used;
}

void TransmissionLineState::transmit(Power amount) {
  // TODO: assert amount < tl->cap - used
  if (tl->capacity == Power::maxPower)
    return;
  used += amount;
}

Power TransmissionLineState::transmittable() const& {
  return tl->capacity - used;
}

std::string TransmissionLineState::toString() const& {
  return fmt::format("{:.2f}", used);
}

} // namespace sgrid