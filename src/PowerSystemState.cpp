#include "PowerSystemState.h"

#include <string>

#include "PowerSystem.h"

Power const PowerSystemState::maxKeeping(400);

PowerSystemState::PowerSystemState(
    PowerSystem* _car, Power _keeping, Power _used
)
    : car(_car), keeping(_keeping), used(_used) {
}

bool PowerSystemState::operator==(PowerSystemState const& other) const& {
  return keeping == other.keeping && used == other.used;
}

PowerSystemState PowerSystemState::fulfill() const& {
  Power amount = std::min(car->capacity - used, keeping);
  return PowerSystemState(car, keeping - amount, used + amount);
}

PowerSystemState PowerSystemState::demand() const& {
  Power amount = car->capacity - used;
  return PowerSystemState(
      car,
      std::min(maxKeeping, keeping + amount),
      used + amount
  );
}

PowerSystemState PowerSystemState::send(Power amount) const& {
  amount = std::min(amount, keeping);
  return PowerSystemState(car, keeping - amount, used);
}

PowerSystemState PowerSystemState::receive(Power amount) const& {
  return PowerSystemState(car, std::min(maxKeeping, keeping + amount), used);
}

std::string PowerSystemState::toString() const& {
  return std::to_string(keeping) + ":" + std::to_string(used);
}
