#include "Grid.h"

#include <vector>

#include "Percentage.h"
#include "PowerSystem.h"
#include "TransmissionLine.h"
#include "utils.h"

namespace sgrid {

Grid::Grid() {
}

Grid::Grid(
    std::vector<PowerSystem> const&      _pss,
    std::vector<TransmissionLine> const& _tls
)
    : pss{_pss}, tls{_tls} {
}

std::string Grid::toString() const& {
  std::string str = "";
  for (auto const& ps: pss)
    str += ps.toString() + ", ";
  str[str.size() - 1] = '\n';
  for (auto const& tl: tls)
    str += tl.toString() + "\n";
  return str;
}

GridFactory&
GridFactory::createPowerSystem(PowerSystemType pst, Power capacity) {
  pss.emplace_back(0, pst, capacity);
  return *this;
}

GridFactory& GridFactory::createTransmissionLine(
    i32 inp, i32 out, Power capacity, Percentage loss
) {
  tls.emplace_back(0, inp, out, capacity, loss);
  return *this;
}

Grid GridFactory::createGrid() {
  for (int sz = pss.size(), i = 0; i < sz; ++i)
    pss[i].id = i;

  for (int sz = tls.size(), i = 0; i < sz; ++i) {
    // for arc contains CON, force direction: CON -> PowerSystem.
    // for arc contains GEN, force direction: PowerSystem -> GEN.
    // for arc that is not one of the aboves, make sure it has both direction.
    // this code will bug out if there is a PowerSystem that is both GEN and CON
    // (which should never happens).
    TransmissionLine& tl = tls[i];
    if (pss[tl.out].pst == PowerSystemType::Consumer)
      std::swap(tl.inp, tl.out);
    if (pss[tl.inp].pst == PowerSystemType::Generator)
      std::swap(tl.inp, tl.out);
    if (pss[tl.inp].pst == PowerSystemType::Consumer)
      continue;
    if (pss[tl.out].pst == PowerSystemType::Generator)
      continue;
    tls.emplace_back(0, tl.out, tl.inp, tl.capacity, tl.loss);
  }

  for (int i = 0; i < tls.size(); ++i)
    tls[i].id = i;

  return Grid{std::move(pss), std::move(tls)};
}

} // namespace sgrid
