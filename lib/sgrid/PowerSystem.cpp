#include <sgrid/PowerSystem.h>

#include <algorithm>
#include <limits.h>
#include <string>

#include <sgrid/Power.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

namespace sgrid {

PowerSystem::PowerSystem(i32 _id, PowerSystemType _pst, Power _capacity)
    : id{_id}, pst{_pst}, capacity{_capacity} {
}

// void PowerSystem::addRevAdjTransmissionLine(TransmissionLine const& tl) {
// }

std::string PowerSystem::toString() const& {
  return std::string(1, static_cast<char>(pst)) + std::to_string(id) + ":" +
         std::to_string(capacity);
}

} // namespace sgrid
