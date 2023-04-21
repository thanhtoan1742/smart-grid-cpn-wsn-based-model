#include <sgrid/Grid.h>

#include <vector>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

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

std::string Grid::idealPathString() const&{
  std::string str = "";
  for (auto const& ps: pss){
    str += ps.idealPathString() + "\n ";
  }
  return str;
}

GridFactory&
GridFactory::createPowerSystem(i32 ref, PowerSystemType pst, Power capacity) {
  pss.emplace_back(ref, pst, capacity);
  return *this;
}

GridFactory& GridFactory::createTransmissionLine(
    i32 inpRef, i32 outRef, Power capacity, Percentage loss
) {
  tls.emplace_back(0, inpRef, outRef, capacity, loss);
  return *this;
}

Grid GridFactory::createGrid() {
  // for every CON or GEN, create an intermediate PowerSystem
  // to force one direction connection to CON or GEN.
  for (int sz = pss.size(), i = 0; i < sz; ++i) {
    if (pss[i].pst == PowerSystemType::Bus)
      continue;

    i32 newPsId = pss.size();
    pss.emplace_back(newPsId, pss[i].pst, pss[i].capacity);
    pss[i].pst      = PowerSystemType::Bus;
    pss[i].capacity = 0;

    if (pss.back().pst == PowerSystemType::Consumer)
      tls.emplace_back(0, newPsId, pss[i].id, Power::maxPower);
    else
      tls.emplace_back(0, pss[i].id, newPsId, Power::maxPower);
  }

  for (int sz = tls.size(), i = 0; i < sz; ++i) {
    // for arc contains CON, force direction: CON -> PowerSystem.
    // for arc contains GEN, force direction: PowerSystem -> GEN.
    // for arc that is not one of the aboves, make sure it has both direction.
    TransmissionLine& tl = tls[i];
    if (pss[tl.inp].pst == PowerSystemType::Consumer)
      continue;
    if (pss[tl.out].pst == PowerSystemType::Generator)
      continue;
    tls.emplace_back(0, tl.out, tl.inp, tl.capacity, tl.loss);
  }

  for (int i = 0; i < tls.size(); ++i) {
    tls[i].id = i;
    TransmissionLine& tl = tls[i];
    pss[tl.inp].addTransmissionLine(tl);
  }

  return Grid{std::move(pss), std::move(tls)};
}

} // namespace sgrid
