#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Carrier.h"
#include "CarrierState.h"
#include "CircuitBreaker.h"
#include "Percentage.h"
#include "Power.h"
#include "State.h"
#include "StateExplorer.h"
#include "utils.h"

void test(CarrierState&& ct) {
}

int main() {

  auto grid = Grid{
      {
       Carrier(0, CarrierType::Consumer, 20),
       Carrier(1, CarrierType::Generator, 30),
       Carrier(2, CarrierType::Generator, 30),
       Carrier(3, CarrierType::Bus),
       },
      {
       CircuitBreaker(0, 1, 3, 1000, 5),
       CircuitBreaker(1, 3, 1, 1000, 5),
       CircuitBreaker(2, 2, 3, 1000, 3),
       CircuitBreaker(3, 3, 2, 1000, 3),
       CircuitBreaker(4, 0, 3, 1000, 2),
       CircuitBreaker(5, 3, 0, 1000, 2),
       }
  };
  StateExplorer stateExplorer(&grid, State(&grid));

  stateExplorer.generateStateSpace();
  // stateExplorer.prettyPrint();
  stateExplorer.prettyPrintBestStateTrace();
  std::cout << stateExplorer.stateSpace.size() << "\n";
}
