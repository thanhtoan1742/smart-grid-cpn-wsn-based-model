#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Carrier.h"
#include "CircuitBreaker.h"
#include "Percentage.h"
#include "Power.h"
#include "State.h"

int sumKeeping(std::vector<Carrier> const& cars) {
  int sum = 0;
  for (auto const& car : cars)
    sum += car.keeping;
  return sum;
}

int main() {
  State s = State {
      {
       Carrier(0, CarrierType::Consumer, 200),
       Carrier(1, CarrierType::Generator, 300),
       },
      {
       CircuitBreaker(0, 0, 1, 1000, 5),
       CircuitBreaker(1, 0, 1, 1000, 5),
       },
  }.demand(0);

  std::cout << s.toString() << "\n";
  for (auto ns: s.generateNextStates()) {
    std::cout << ns.toString() << "\n";
  }
}
