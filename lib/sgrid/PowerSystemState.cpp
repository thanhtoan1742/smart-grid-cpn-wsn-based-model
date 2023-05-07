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
  keeping -= amount;
  used += amount;
}

PowerSystemState PowerSystemState::fulfilled() const& {
  Power amount = std::min(ps->capacity - used, keeping);
  return PowerSystemState(ps, keeping - amount, used + amount);
}

PowerSystemState PowerSystemState::sent(Power amount) const& {
  amount = std::min(amount, keeping);
  return PowerSystemState(ps, keeping - amount, used);
}

PowerSystemState PowerSystemState::received(Power amount) const& {
  return PowerSystemState(ps, keeping + amount, used);
}

Power PowerSystemState::fulfillable() const& {
  return ps->capacity - used;
}

std::string PowerSystemState::toString() const& {
  return std::to_string(keeping) + ":" + std::to_string(used);
}

} // namespace sgrid
