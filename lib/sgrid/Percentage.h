#ifndef PERCENTAGE_H
#define PERCENTAGE_H

#include <lib/sgrid/Types.h>

namespace sgrid {

using Percentage = u64;
Percentage operator""_pct(unsigned long long);

} // namespace sgrid
#endif // PERCENTAGE_H
