#include "Power.h"
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
  return value * percentage / 100;
}

void Power::operator+=(const Power &other) & {
  value += other.value;
}

void Power::operator-=(const Power &other) & {
  value -= other.value;
}

void Power::operator*=(const Percentage &percentage) & {
  value = value * static_cast<u64>(percentage) / 100;
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
