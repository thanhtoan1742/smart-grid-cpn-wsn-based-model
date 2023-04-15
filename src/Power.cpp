#include "Power.h"

#include <algorithm>

#include "Percentage.h"

Power::Power(u64 _value): value{_value} {
}

Power Power::operator+(Power const& other) const& {
  return Power(value + other.value);
}

Power Power::operator-(Power const& other) const& {
  return Power(value - other.value);
}

Power Power::operator*(Percentage const& percentage) const& {
  return (value * percentage + 99) / 100;
}

void Power::operator+=(Power const& other) & {
  value += other.value;
}

void Power::operator-=(Power const& other) & {
  value -= other.value;
}

void Power::operator*=(Percentage const& percentage) & {
  value = (value * percentage);
}

bool Power::operator<(Power const& other) const& {
  return value < other.value;
}

Power Power::compensateLoss(Percentage const& percentage) const& {
  return (*this) * (100_pct + percentage);
}

Power::operator u64() const& {
  return value;
}

Power operator""_pu(u64 value) {
  return Power(value);
}