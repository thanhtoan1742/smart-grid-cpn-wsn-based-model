#include <sgrid/Grid.h>

#include <sgrid/Percentage.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

#include <fmt/core.h>
#include <plog/Log.h>

#include <cassert>
#include <vector>

namespace sgrid {

Grid::Grid() {
}

Grid::Grid(
    std::vector<PowerSystem*> const&      _pss,
    std::vector<TransmissionLine*> const& _tls
)
    : pss{_pss}, tls{_tls} {
  for (auto tl: tls) {
    tl->out->revAdj.push_back(tl);
  }
}

Grid::~Grid() {
  for (auto ps: pss)
    delete ps;
  for (auto tl: tls)
    delete tl;
}

std::string Grid::toString() const& {
  std::string str = "";
  for (auto ps: pss)
    str += ps->toString() + ", ";
  str[str.size() - 1] = '\n';
  for (auto tl: tls)
    str += tl->toString() + "\n";
  return str;
}

GridFactory&
GridFactory::createPowerSystem(i32 ref, PowerSystemType pst, Power capacity) {
  assert(ref == ref2ps.size());
  pss.push_back(new PowerSystem(ref, pst, capacity));
  ref2ps.emplace(ref, pss.back());
  return *this;
}

GridFactory& GridFactory::createTransmissionLine(
    i32 inpRef, i32 outRef, Power capacity, Percentage loss
) {
  assert(ref2ps.count(inpRef));
  assert(ref2ps.count(outRef));
  tls.emplace_back(
      new TransmissionLine(0, ref2ps[inpRef], ref2ps[outRef], capacity, loss)
  );
  return *this;
}

Grid GridFactory::createGrid() {
  // for every CON or GEN, create an intermediate PowerSystem
  // to force one direction connection to CON or GEN.
  for (int sz = pss.size(), i = 0; i < sz; ++i) {
    if (pss[i]->pst == PowerSystemType::Bus)
      continue;

    i32 newPsId = pss.size();
    createPowerSystem(newPsId, pss[i]->pst, pss[i]->capacity);
    pss[i]->pst      = PowerSystemType::Bus;
    pss[i]->capacity = 0;

    if (pss.back()->pst == PowerSystemType::Consumer)
      createTransmissionLine(newPsId, pss[i]->id, Power::maxPower);
    else
      createTransmissionLine(pss[i]->id, newPsId, Power::maxPower);
  }

  for (int sz = tls.size(), i = 0; i < sz; ++i) {
    // for arc contains CON, force direction: CON -> PowerSystem.
    // for arc contains GEN, force direction: PowerSystem -> GEN.
    // for arc that is not one of the aboves, make sure it has both direction.
    auto tl = tls[i];
    if (tl->inp->pst == PowerSystemType::Consumer)
      continue;
    if (tl->out->pst == PowerSystemType::Generator)
      continue;
    createTransmissionLine(tl->out->id, tl->inp->id, tl->capacity, tl->loss);
  }

  for (int i = 0; i < tls.size(); ++i) {
    tls[i]->id   = i;
    tls[i]->loss = tls[i]->loss + 100_pct;
  }

  return Grid(std::move(pss), std::move(tls));
}

} // namespace sgrid
