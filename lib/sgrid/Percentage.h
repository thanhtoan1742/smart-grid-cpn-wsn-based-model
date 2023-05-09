#ifndef SGRID_PERCENTAGE_H
#define SGRID_PERCENTAGE_H

#include <sgrid/types.h>

#include <numeric>
#include <string>

namespace sgrid {

struct Percentage {
  f64 value;

  Percentage(f64 value);
  Percentage operator*(Percentage other);
  Percentage operator+(Percentage other);

  bool operator<(Percentage other);
  bool operator>(Percentage other);

  std::string toString() const&;
};

Percentage operator""_pct(unsigned long long);

} // namespace sgrid
#endif // SGRID_PERCENTAGE_H
