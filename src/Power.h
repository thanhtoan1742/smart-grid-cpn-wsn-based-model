#ifndef POWER_H
#define POWER_H

#include "Percentage.h"

struct Power {
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

Power operator""_pu(u64 value);

#endif // POWER_H
