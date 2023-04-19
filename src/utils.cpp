#include "utils.h"

void _debug(State const& state) {
  std::cerr << state.toString() << " ";
  std::cerr << "parent:" << state.parent << " ";
  std::cerr << "children_size:" << state.children.size() << " ";
}

std::string padded(std::string str, int size, char padder) {
  while (str.size() < size)
    str += padder;
  return str;
}