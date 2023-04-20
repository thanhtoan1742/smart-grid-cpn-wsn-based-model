#include <libsgrid/PowerSystem.h>

#include <string>

#include <libsgrid/Power.h>

namespace sgrid {

PowerSystem::PowerSystem(i32 _id, PowerSystemType _pst, Power _capacity)
    : id{_id}, pst{_pst}, capacity{_capacity} {
}

std::string PowerSystem::toString() const& {
  return std::string(1, static_cast<char>(pst)) + std::to_string(id) + ":" +
         std::to_string(capacity);
}

} // namespace sgrid
