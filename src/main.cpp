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

  // auto grid = Grid{
  //     {
  //      Carrier(0, CarrierType::Consumer, 20),
  //      Carrier(1, CarrierType::Generator, 30),
  //      Carrier(2, CarrierType::Generator, 30),
  //      Carrier(3, CarrierType::Bus),
  //      },
  //     {
  //      CircuitBreaker(0, 1, 3, 1000, 5),
  //      CircuitBreaker(1, 3, 1, 1000, 5),
  //      CircuitBreaker(2, 2, 3, 1000, 3),
  //      CircuitBreaker(3, 3, 2, 1000, 3),
  //      CircuitBreaker(4, 0, 3, 1000, 2),
  //      CircuitBreaker(5, 3, 0, 1000, 2),
  //      }
  // };
  // auto grid = Grid{
  //     {
  //      Carrier(0, CarrierType::Consumer, 10),
  //      Carrier(1, CarrierType::Generator, 20),
  //      Carrier(2, CarrierType::Bus),
  //      },
  //     {
  //      CircuitBreaker(0, 1, 2, 1000, 20),
  //      CircuitBreaker(1, 2, 1, 1000, 20),
  //      CircuitBreaker(2, 2, 0, 1000, 10),
  //      CircuitBreaker(3, 0, 2, 1000, 10),
  //      }
  // };

  auto grid = Grid{
      {
       Carrier(0, CarrierType::Consumer, 10),
       Carrier(1, CarrierType::Consumer, 10),
       Carrier(2, CarrierType::Bus),
       Carrier(3, CarrierType::Bus),
       Carrier(4, CarrierType::Bus),
       Carrier(5, CarrierType::Bus),
       Carrier(6, CarrierType::Generator, 50),
       Carrier(7, CarrierType::Generator, 50),
       },
      {
       CircuitBreaker(0, 0, 3, 1000, 0),
       CircuitBreaker(1, 3, 0, 1000, 0),
       CircuitBreaker(2, 1, 2, 1000, 0),
       CircuitBreaker(3, 2, 1, 1000, 0),
       CircuitBreaker(4, 3, 2, 1000, 10),
       CircuitBreaker(5, 2, 3, 1000, 10),
       CircuitBreaker(6, 3, 4, 1000, 10),
       CircuitBreaker(7, 4, 3, 1000, 10),
       CircuitBreaker(8, 2, 5, 1000, 30),
       CircuitBreaker(9, 5, 2, 1000, 30),
       CircuitBreaker(10, 4, 5, 1000, 10),
       CircuitBreaker(11, 5, 4, 1000, 10),
       CircuitBreaker(12, 5, 6, 1000, 10),
       CircuitBreaker(13, 6, 5, 1000, 10),
       CircuitBreaker(14, 4, 7, 1000, 20),
       CircuitBreaker(15, 7, 4, 1000, 20),
       }
  };
  StateExplorer stateExplorer(&grid, State(&grid));

  stateExplorer.generateStateSpace();
  // stateExplorer.prettyPrint();
  stateExplorer.prettyPrintBestStateTrace();
  std::cout << stateExplorer.stateSpace.size() << "\n";
}
