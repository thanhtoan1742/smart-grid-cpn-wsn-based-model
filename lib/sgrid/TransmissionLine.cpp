#include <sgrid/TransmissionLine.h>

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>

#include <fmt/core.h>

#include <string>

namespace sgrid {

TransmissionLine::TransmissionLine(
    i32          _id,
    PowerSystem* _inp,
    PowerSystem* _out,
    Power        _capacity,
    Percentage   _loss
)
    : id{_id}, inp{_inp}, out{_out}, capacity{_capacity}, loss{_loss} {
}

std::string TransmissionLine::toString() const& {
  return fmt::format(
      "{}->{}({}:{})",
      inp->id,
      out->id,
      capacity,
      loss.toString()
  );
}

} // namespace sgrid
