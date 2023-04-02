#include "utils.h"

void _debug(State const& state) {
  std::cerr << state.toString() << " ";
  std::cerr << "parent:" << state.parent << " ";
  std::cerr << "children_size:" << state.children.size() << " ";
}
