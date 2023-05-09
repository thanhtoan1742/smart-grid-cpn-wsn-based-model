#include <sgrid/Power.h>

#include <sgrid/Percentage.h>
#include <sgrid/types.h>

#include <algorithm>
#include <cstdlib>

namespace sgrid {

Power const Power::maxPower(1e6);
Power const Power::zero(0);
f64 const   Power::epsilon = 1e-2;

Power::Power(f64 _value): value{_value} {
}

Power Power::operator+(Power const& other) const& {
  return Power(value + other.value);
}

Power Power::operator-(Power const& other) const& {
  return Power(value - other.value);
}

Power Power::operator*(Percentage const& percentage) const& {
  return Power(value * percentage.value);
}

Power Power::operator/(Percentage const& percentage) const& {
  return Power(value / percentage.value);
}

void Power::operator+=(Power const& other) & {
  value += other.value;
}

void Power::operator-=(Power const& other) & {
  value -= other.value;
}

void Power::operator*=(Percentage const& percentage) & {
  (*this) = (*this) * percentage;
}

bool Power::operator<(Power const& other) const& {
  return value < other.value;
}

bool Power::operator==(Power const& other) const& {
  return fabs(value - other.value) < epsilon;
}

Power::operator f64() const& {
  return value;
}

Power operator""_pu(unsigned long long value) {
  return Power(static_cast<f64>(value));
}

} // namespace sgrid
