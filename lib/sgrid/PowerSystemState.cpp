#include <sgrid/PowerSystemState.h>

#include <string>

#include <sgrid/PowerSystem.h>

namespace sgrid {

PowerSystemState::PowerSystemState(
    PowerSystem* _ps, Power _keeping, Power _used
)
    : ps(_ps), keeping(_keeping), used(_used) {
      remain_cap = ps->capacity;
}

bool PowerSystemState::operator==(PowerSystemState const& other) const& {
  return keeping == other.keeping && used == other.used;
}

PowerSystemState PowerSystemState::fulfill() const& {
  Power amount = std::min(ps->capacity - used, keeping);
  return PowerSystemState(ps, keeping - amount, used + amount);
}

PowerSystemState PowerSystemState::send(Power amount) const& {
  amount = std::min(amount, keeping);
  return PowerSystemState(ps, keeping - amount, used);
}

PowerSystemState PowerSystemState::receive(Power amount) const& {
  return PowerSystemState(ps, keeping + amount, used);
}

std::string PowerSystemState::toString() const& {
  return std::to_string(keeping) + ":" + std::to_string(used);
}

} // namespace sgrid
