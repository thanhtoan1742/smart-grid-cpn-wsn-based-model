#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Carrier.h"
#include "CircuitBreaker.h"
#include "State.h"

int sumKeeping(std::vector<Carrier> const& cars) {
  int sum = 0;
  for (auto const& car : cars)
    sum += car.keeping;
  return sum;
}

int main() {
  State s;
  s.cars.emplace_back(s.cars.size(), CarrierType::Consumer, 200);
  s.cars.emplace_back(s.cars.size(), CarrierType::Generator, 300);
  s.cbs.emplace_back(s.cbs.size(), 0, 1, 1000, 5);
  s.cbs.emplace_back(s.cbs.size(), 1, 0, 1000, 5);

  std::cout << "id:keeping:used\n";
  std::stack<State>     q;
  std::set<std::string> inQueue;

  q.push(s);
  inQueue.insert(s.toString());
  while (!q.empty()) {
    State s = q.top();
    q.pop();

    for (int i = 0; i < s.depth; ++i)
      std::cout << " ";
    std::cout << s.toString() << "\n";

    for (auto& ns : s.generateNextStates()) {
      if (sumKeeping(ns.cars) > 1000)
        continue;
      std::string str = ns.toString();
      if (inQueue.count(str))
        continue;
      ns.depth = s.depth + 1;
      inQueue.insert(str);
      q.push(ns);
    }
  }
}
