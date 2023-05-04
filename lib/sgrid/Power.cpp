#include <sgrid/Power.h>

#include <sgrid/Percentage.h>

#include <algorithm>

namespace sgrid {

Power const Power::maxPower(1'000'000);

Power::Power(u64 _value): value{_value} {
}

Power Power::operator+(Power const& other) const& {
  return Power(value + other.value);
}

Power Power::operator-(Power const& other) const& {
  return Power(value - other.value);
}

Power Power::operator*(Percentage const& percentage) const& {
  return Power(
      (value * percentage.value + Percentage::base - 1) / Percentage::base
  );
}

Power Power::operator/(Percentage const& percentage) const& {
  return Power(value * Percentage::base / percentage.value);
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

Power::operator u64() const& {
  return value;
}

Power operator""_pu(unsigned long long value) {
  return Power(static_cast<u64>(value));
}

} // namespace sgrid
