#include <sgrid/TransmissionLine.h>

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>

#include <string>

namespace sgrid {

TransmissionLine::TransmissionLine(
    i32 _id, i32 _inp, i32 _out, Power _capacity, Percentage _loss
)
    : id{_id}, inp{_inp}, out{_out}, capacity{_capacity}, loss{_loss} {
}

std::string TransmissionLine::toString() const& {
  return std::to_string(id) + ":" + std::to_string(inp) + "--" +
         std::to_string(capacity) + "|" + loss.toString() + "->" +
         std::to_string(out);
}

} // namespace sgrid
