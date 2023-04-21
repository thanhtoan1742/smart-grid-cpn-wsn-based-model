#ifndef PERCENTAGE_H
#define PERCENTAGE_H

#include <numeric>
#include <string>

#include <sgrid/Types.h>

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
#endif // PERCENTAGE_H
