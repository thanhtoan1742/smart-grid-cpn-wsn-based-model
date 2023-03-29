#include "Carrier.h"
#include "Power.h"

Carrier::Carrier(
    int _id, CarrierType _ct, Power _capacity, Power _keeping, Power _used
)
    : id{_id}, ct{_ct}, capacity{_capacity}, keeping{_keeping}, used{_used} {
}

std::string Carrier::toString() const& {
  return std::string(1, static_cast<char>(ct)) + std::to_string(id) + ":" +
         std::to_string(keeping) + ":" + std::to_string(used);
}
