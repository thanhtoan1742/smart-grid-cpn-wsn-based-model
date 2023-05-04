#ifndef SGRID_UTILS_H
#define SGRID_UTILS_H

#include <sgrid/State.h>

#include <iostream>
#include <vector>

namespace sgrid {

void _debug(State const& state);

template <typename T>
void _debug(T const& arg) {
  std::cerr << arg << " ";
}

template <typename T, typename... Ts>
void _debug(T const& first, Ts const&... rest) {
  _debug(first);
  _debug(rest...);
}

template <typename... Ts>
void debug(Ts... args) {
  _debug("DEBUG: ");
  _debug(args...);
  std::cerr << std::endl;
}

std::string padded(std::string str, int size = 10, char padder = ' ');

} // namespace sgrid
#endif // SGRID_UTILS_H
