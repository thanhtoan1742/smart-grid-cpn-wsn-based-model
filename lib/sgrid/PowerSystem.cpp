#include <sgrid/PowerSystem.h>

#include <algorithm>
#include <limits.h>
#include <string>

#include <sgrid/Power.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

namespace sgrid {

IdealPath::IdealPath(int nextId, i32 genIdx, Percentage loss)
    : nextId{nextId},genIdx(genIdx), loss{loss} {
}

std::string IdealPath::toString() const& {
  return "nextId: " + std::to_string(nextId) + "|" + "genIdx:" + std::to_string(genIdx) + "|" + "loss:" + std::to_string(loss);
}

PowerSystem::PowerSystem(i32 _id, PowerSystemType _pst, Power _capacity)
    : id{_id}, pst{_pst}, capacity{_capacity} {
}

void PowerSystem::addTransmissionLine(TransmissionLine tl) {
  tls.emplace_back(tl);
}

void PowerSystem::updateIdealNeighbor(int tlIdx, int idx, i32 genIdx, Percentage value){
  // debug("psId:", toString(), "idx: ", idx, "size: ", std::to_string(idealPaths.size()));
  while (tlIdx >= idealPaths.size())
    idealPaths.emplace_back(new IdealPath(idx, genIdx, value));
  if (idealPaths[tlIdx]->loss > value){
    idealPaths[tlIdx]->loss = value;
    idealPaths[tlIdx]->nextId = idx;
  }
}

std::vector<int> PowerSystem::getIdealPathOrder(){
  std::vector<int> order;
  for (int i = 0; i < idealPaths.size(); i++){
    order.emplace_back(i);
  }
  std::sort(order.begin(), order.end(), [&](int a, int b){
    return idealPaths[a]->loss < idealPaths[b]->loss;
  });
  return order;
}

std::string PowerSystem::toString() const& {
  return std::string(1, static_cast<char>(pst)) + std::to_string(id) + ":" +
         std::to_string(capacity) + "_genIdx:" + std::to_string(genIdx);
}

std::string PowerSystem::IdealPathToString() const& {
  std::string str = toString() + "\n";
  for (auto const& ip: idealPaths){
    str += "\t" + ip->toString() + "\n ";
  }
  return str;
}

} // namespace sgrid
