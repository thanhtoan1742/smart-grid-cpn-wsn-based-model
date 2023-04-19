#include "CarrierState.h"

#include <string>

#include "PowerSystem.h"

Power const CarrierState::maxKeeping(400);

CarrierState::CarrierState(PowerSystem* _car, Power _keeping, Power _used)
    : car(_car), keeping(_keeping), used(_used) {
}

bool CarrierState::operator==(CarrierState const& other) const& {
  return keeping == other.keeping && used == other.used;
}

CarrierState CarrierState::fulfill() const& {
  Power amount = std::min(car->capacity - used, keeping);
  return CarrierState(car, keeping - amount, used + amount);
}

CarrierState CarrierState::demand() const& {
  Power amount = car->capacity - used;
  return CarrierState(
      car,
      std::min(maxKeeping, keeping + amount),
      used + amount
  );
}

CarrierState CarrierState::send(Power amount) const& {
  amount = std::min(amount, keeping);
  return CarrierState(car, keeping - amount, used);
}

CarrierState CarrierState::receive(Power amount) const& {
  return CarrierState(car, std::min(maxKeeping, keeping + amount), used);
}

std::string CarrierState::toString() const& {
  return std::to_string(keeping) + ":" + std::to_string(used);
}
