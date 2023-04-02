#include "CircuitBreaker.h"

#include "Percentage.h"
#include "Power.h"
#include <string>

CircuitBreaker::CircuitBreaker(
    i32 _id, i32 _inp, i32 _out, Power _capacity, Percentage _loss
)
    : id{_id}, inp{_inp}, out{_out}, capacity{_capacity}, loss{_loss} {
}

std::string CircuitBreaker::toString() const& {
  return std::to_string(id) + ":" + std::to_string(inp) + "--" +
         std::to_string(capacity) + "|" + std::to_string(loss) + "->" +
         std::to_string(out);
}
