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
       Carrier(0, CarrierType::Consumer, 200),
       Carrier(1, CarrierType::Generator, 300),
       },
      {
       CircuitBreaker(0, 0, 1, 1000, 0),
       CircuitBreaker(1, 1, 0, 1000, 0),
       }
  };
  StateExplorer stateExplorer(&grid, State(&grid));

  stateExplorer.generateStateSpace();
  // stateExplorer.prettyPrint();
  stateExplorer.prettyPrintBestStateTrace();
}
