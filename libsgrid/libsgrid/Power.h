#ifndef POWER_H
#define POWER_H

#include <libsgrid/Percentage.h>

namespace sgrid {

struct Power {
  static Power const maxPower;

  Power(u64 value = 0);

  Power operator+(Power const& other) const&;
  Power operator-(Power const& other) const&;
  Power operator*(Percentage const& percentage) const&;
  void  operator+=(Power const& other) &;
  void  operator-=(Power const& other) &;
  void  operator*=(Percentage const& other) &;

  bool operator<(Power const& other) const&;

  Power compensateLoss(Percentage const& percentage) const&;

  operator u64() const&;

private:
  u64 value;
};

Power operator""_pu(unsigned long long value);

} // namespace sgrid
#endif // POWER_H
