#include <sgrid/PowerSystemState.h>

#include <sgrid/PowerSystem.h>

#include <string>

namespace sgrid {

PowerSystemState::PowerSystemState(
    PowerSystem* _ps, Power _keeping, Power _used
)
    : ps(_ps), keeping(_keeping), used(_used) {
}

bool PowerSystemState::operator==(PowerSystemState const& other) const& {
  return keeping == other.keeping && used == other.used;
}

void PowerSystemState::fulfill() {
  Power amount = std::min(ps->capacity - used, keeping);
  keeping      -= amount;
  used         += amount;
}

void PowerSystemState::send(Power amount) {
  keeping -= std::min(amount, keeping);
}

void PowerSystemState::receive(Power amount) {
  keeping += amount;
}

Power PowerSystemState::fulfillable() const& {
  return ps->capacity - used;
}

std::string PowerSystemState::toString() const& {
  return std::to_string(keeping) + ":" + std::to_string(used);
}

} // namespace sgrid
