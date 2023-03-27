#include "Carrier.h"

Carrier::Carrier(
    int _id, CarrierType _ct, int _capacity, int _keeping, int _used
)
    : id{_id}, ct{_ct}, capacity{_capacity}, keeping{_keeping}, used{_used} {
}

std::string Carrier::toString() const& {
  return std::string(1, static_cast<char>(ct)) + std::to_string(id) + ":" +
         std::to_string(keeping) + ":" + std::to_string(used);
}
