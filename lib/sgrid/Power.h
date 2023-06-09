#ifndef SGRID_POWER_H
#define SGRID_POWER_H

#include <sgrid/Percentage.h>

namespace sgrid {

struct Power {
  static Power const maxPower;
  static Power const zero;
  static f64 const   epsilon;

  Power(f64 value = 0);

  Power operator+(Power const& other) const&;
  Power operator-(Power const& other) const&;
  Power operator*(Percentage const& percentage) const&;
  Power operator/(Percentage const& percentage) const&;
  void  operator+=(Power const& other) &;
  void  operator-=(Power const& other) &;
  void  operator*=(Percentage const& other) &;

  bool operator<(Power const& other) const&;
  bool operator==(Power const& other) const&;

  operator f64() const&;

private:
  f64 value;
};

Power operator""_pu(unsigned long long value);

} // namespace sgrid
#endif // SGRID_POWER_H
