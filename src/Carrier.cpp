#include "Carrier.h"
#include "Power.h"
#include <string>

Carrier::Carrier(i32 _id, CarrierType _ct, Power _capacity)
    : id{_id}, ct{_ct}, capacity{_capacity} {
}

std::string Carrier::toString() const& {
  return std::string(1, static_cast<char>(ct)) + std::to_string(id) + ":" +
         std::to_string(capacity);
}
