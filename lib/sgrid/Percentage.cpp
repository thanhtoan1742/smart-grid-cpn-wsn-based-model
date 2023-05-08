#include <sgrid/Percentage.h>

#include <fmt/core.h>

#include <string>

namespace sgrid {

u64 const Percentage::base = 100;

Percentage::Percentage(u64 _value): value(_value * base / 100) {
}

Percentage Percentage::operator+(Percentage other) {
  return Percentage(value + other.value);
}

Percentage Percentage::operator*(Percentage other) {
  return Percentage(value * other.value / base);
}

bool Percentage::operator<(Percentage other) {
  return value < other.value;
}

bool Percentage::operator>(Percentage other) {
  return value > other.value;
}

bool Percentage::operator==(Percentage other) {
  return value == other.value;
}

std::string Percentage::toString() const& {
  return fmt::format("{}%", value);
}

Percentage operator""_pct(unsigned long long value) {
  return Percentage(static_cast<u64>(value));
}

} // namespace sgrid
