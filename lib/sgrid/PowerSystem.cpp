#include <sgrid/PowerSystem.h>

#include <sgrid/Power.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

#include <fmt/core.h>

#include <algorithm>
#include <limits.h>
#include <string>

namespace sgrid {

PowerSystem::PowerSystem(i32 _id, PowerSystemType _pst, Power _capacity)
    : id{_id}, pst{_pst}, capacity{_capacity} {
}

std::string PowerSystem::toString() const& {
  return fmt::format("{}{}:{}", static_cast<char>(pst), id, capacity);
}

} // namespace sgrid
