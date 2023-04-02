#ifndef UTILS_H
#define UTILS_H

#include "State.h"
#include <iostream>
#include <vector>

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

#endif // UTILS_H
