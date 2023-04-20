#include <lib/sgrid/Percentage.h>

namespace sgrid {

Percentage operator""_pct(unsigned long long value) {
  return static_cast<u64>(value);
}

} // namespace sgrid
