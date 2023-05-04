#ifndef SGRID_PERCENTAGE_H
#define SGRID_PERCENTAGE_H

#include <sgrid/Types.h>

#include <numeric>
#include <string>

namespace sgrid {

struct Percentage {
  static u64 const base;

  u64 value;

  Percentage(u64 value);
  Percentage operator*(Percentage other);
  Percentage operator+(Percentage other);

  bool operator<(Percentage other);
  bool operator>(Percentage other);
  bool operator==(Percentage other);

  std::string toString() const&;
};

Percentage operator""_pct(unsigned long long);

} // namespace sgrid
#endif // SGRID_PERCENTAGE_H
