#include "PowerSystem.h"

#include <string>

#include "Power.h"

PowerSystem::PowerSystem(i32 _id, PowerSystemType _ct, Power _capacity)
    : id{_id}, ct{_ct}, capacity{_capacity} {
}

std::string PowerSystem::toString() const& {
  return std::string(1, static_cast<char>(ct)) + std::to_string(id) + ":" +
         std::to_string(capacity);
}
