#include <sgrid/State.h>

#include <iterator>
#include <limits.h>
#include <string>
#include <unordered_set>
#include <queue>
#include <vector>


#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/utils.h>

namespace sgrid {

State::State(Grid* _grid): grid(_grid), depth(0), parent(nullptr) {
  for (PowerSystem& car: grid->pss)
    psStates.emplace_back(&car);
    updateIdealPath();
}

State::State(Grid* _grid, std::vector<PowerSystemState> const& _psStates)
    : grid(_grid), psStates(_psStates) {
    updateIdealPath();
}

bool State::operator==(State const& other) const& {
  if (psStates.size() != other.psStates.size())
    return false;
  for (int i = 0; i < psStates.size(); ++i)
    if (psStates[i] != other.psStates[i])
      return false;
  return true;
}

bool State::satisfied() const& {
  for (int i = 0; i < grid->pss.size(); ++i) {
    PowerSystem const& ps = grid->pss[i];
    if (ps.pst == PowerSystemType::Consumer && psStates[i].used < ps.capacity)
      return false;
    if (psStates[i].keeping != 0)
      return false;
  }
  return true;
}

Power State::notDemaned() const& {
  Power res(0);
  for (int i = 0; i < grid->pss.size(); ++i)
    if (grid->pss[i].pst == PowerSystemType::Consumer)
      res += grid->pss[i].capacity - psStates[i].used;
  return res;
}

Power State::fulfilled() const& {
  Power res(0);
  for (int i = 0; i < grid->pss.size(); ++i)
    if (grid->pss[i].pst == PowerSystemType::Generator)
      res += psStates[i].used;
  return res;
}

Power State::needFulfilled() const& {
  Power res(0);
  for (int i = 0; i < grid->pss.size(); ++i)
    if (grid->pss[i].pst == PowerSystemType::Generator) {
      Power need = grid->pss[i].capacity - psStates[i].used;
      res        += std::max(0_pu, psStates[i].keeping - need);
    }
  return res;
}

Power State::keeping() const& {
  Power res(0);
  for (auto const& psState: psStates)
    res += psState.keeping;
  return res;
}

State State::createChildState(i32 idx, PowerSystemState const& newPsState)
    const& {
  std::vector<PowerSystemState> newPsStates(psStates);
  newPsStates[idx] = std::move(newPsState);
  return State(grid, newPsStates);
}

State State::demand() const& {
  State newState(*this);
  for (auto& psState: newState.psStates)
    if (psState.ps->pst == PowerSystemType::Consumer) {
      psState.keeping = psState.ps->capacity;
      psState.used    = psState.ps->capacity;
    }
  return newState;
}

std::vector<State> State::generateNextStates() const& {
  std::unordered_set<State> uniqueStates;
  for (int i = 0; i < grid->pss.size(); ++i) {
    if (grid->pss[i].pst == PowerSystemType::Generator)
      uniqueStates.insert(createChildState(i, psStates[i].fulfill()));
  }

  for (int i = 0; i < grid->tls.size(); ++i) {
    i32        inp  = grid->tls[i].inp;
    i32        out  = grid->tls[i].out;
    Percentage loss = grid->tls[i].loss;

    Power maxAmount = std::min(psStates[inp].keeping, grid->tls[i].capacity);
    for (Power amount = maxAmount; amount > 0_pu; amount -= 1) {
      State nextState(*this);
      nextState.psStates[inp] = nextState.psStates[inp].send(amount);
      nextState.psStates[out] =
          nextState.psStates[out].receive(amount.compensateLoss(loss));
      uniqueStates.insert(nextState);
      break; // only try maxAmount
    }
  }

  return std::vector<State>(
      std::make_move_iterator(uniqueStates.begin()),
      std::make_move_iterator(uniqueStates.end())
  );
}

struct LossComparator {
  bool operator()(std::pair<int, Percentage> const& a,
                  std::pair<int, Percentage> const& b) const& {
    return a.second < b.second;
  }
};

void State::updateIdealPath(){
  debug("RUN updateIdealPath\n");
  std::priority_queue<std::pair<int, Percentage>, std::vector<std::pair<int, Percentage>>, LossComparator>  q;
  std::vector<Percentage> minLoss(psStates.size(), ULLONG_MAX);

  for(int i = 0; i < grid->tls.size(); i++){
    i32       inp  = grid->tls[i].inp;
    i32       out  = grid->tls[i].out;
    if (grid->pss[out].pst == PowerSystemType::Generator && psStates[out].remain_cap > 0){
      grid->pss[inp].genIdx = out;
      minLoss[inp] = grid->tls[i].loss;
      q.push(std::pair<int, Percentage>(inp, grid->tls[i].loss));
    }
  }

  while (!q.empty()){
    auto [idx, loss] = q.top();
    q.pop();
    if (minLoss[idx] != loss)
      continue;
    // debug("pop: " + std::to_string(idx) + " " + std::to_string(loss) + "\n");
    
    for (int i = 0; i < grid->pss[idx].tls.size(); i++){
      TransmissionLine tl = grid->pss[idx].tls[i];
      int out = tl.out;
      grid->pss[out].updateIdealNeighbor(i, idx, grid->pss[idx].genIdx, loss + tl.loss);
      if (minLoss[out] > loss + tl.loss){
        grid->pss[out].genIdx = grid->pss[idx].genIdx;
        minLoss[out] = loss + tl.loss;
        q.push(std::pair<int, Percentage>(out, minLoss[out]));
      }
    }
  }
}

std::string State::toString() const& {
  if (psStates.empty())
    return "[]";
  std::string str = "[";
  for (auto const& psState: psStates)
    str +=
        padded(std::to_string(psState.ps->id) + "|" + psState.toString() + " ");
  str[str.size() - 1] = ']';
  return str;
}

} // namespace sgrid

std::size_t std::hash<sgrid::State>::operator()(sgrid::State const& state
) const noexcept {
  std::string str;
  for (auto const& psState: state.psStates)
    str += std::to_string(psState.keeping) + ":" +
           std::to_string(psState.used) + " ";
  return std::hash<std::string>()(str);
}