#include <sgrid/Percentage.h>

#include <fmt/core.h>

#include <string>

namespace sgrid {

Percentage::Percentage(f64 _value): value(_value) {
}

Percentage Percentage::operator+(Percentage other) {
  return Percentage(value + other.value);
}

Percentage Percentage::operator*(Percentage other) {
  return Percentage(value * other.value);
}

bool Percentage::operator<(Percentage other) {
  return value < other.value;
}

bool Percentage::operator>(Percentage other) {
  return value > other.value;
}

std::string Percentage::toString() const& {
  return fmt::format("{:.2f}%", value * 100);
}

Percentage operator""_pct(unsigned long long value) {
  return Percentage(static_cast<f64>(value) / 100);
}

} // namespace sgrid
