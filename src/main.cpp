#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "CircuitBreaker.h"
#include "Grid.h"
#include "Percentage.h"
#include "Power.h"
#include "PowerSystem.h"
#include "PowerSystemState.h"
#include "State.h"
#include "StateExplorer.h"
#include "utils.h"

void test(PowerSystemState&& ct) {
}

int main() {
  // auto grid = Grid{
  //     {
  //      PowerSystem(0, PowerSystemType::Consumer, 10),
  //      PowerSystem(1, PowerSystemType::Consumer, 10),
  //      PowerSystem(2, PowerSystemType::Bus),
  //      PowerSystem(3, PowerSystemType::Bus),
  //      PowerSystem(4, PowerSystemType::Bus),
  //      PowerSystem(5, PowerSystemType::Bus),
  //      PowerSystem(6, PowerSystemType::Generator, 50),
  //      PowerSystem(7, PowerSystemType::Generator, 50),
  //      },
  //     {
  //      CircuitBreaker(0, 0, 3, 1000, 0),
  //      CircuitBreaker(1, 3, 0, 1000, 0),
  //      CircuitBreaker(2, 1, 2, 1000, 0),
  //      CircuitBreaker(3, 2, 1, 1000, 0),
  //      CircuitBreaker(4, 3, 2, 1000, 10),
  //      CircuitBreaker(5, 2, 3, 1000, 10),
  //      CircuitBreaker(6, 3, 4, 1000, 10),
  //      CircuitBreaker(7, 4, 3, 1000, 10),
  //      CircuitBreaker(8, 2, 5, 1000, 30),
  //      CircuitBreaker(9, 5, 2, 1000, 30),
  //      CircuitBreaker(10, 4, 5, 1000, 10),
  //      CircuitBreaker(11, 5, 4, 1000, 10),
  //      CircuitBreaker(12, 5, 6, 1000, 10),
  //      CircuitBreaker(13, 6, 5, 1000, 10),
  //      CircuitBreaker(14, 4, 7, 1000, 20),
  //      CircuitBreaker(15, 7, 4, 1000, 20),
  //      }
  // };

  /*
   *    C0-----------------------B3-----------B6-------G10
   *                             |
   *                             |
   *    C1-----------B4----------B7-----------G9
   *                 |
   *                 |
   *    C2-----------B5----------G8
   *
   * */
  auto grid = Grid{
      {
       PowerSystem(0, PowerSystemType::Consumer, 50),
       PowerSystem(1, PowerSystemType::Consumer, 50),
       PowerSystem(2, PowerSystemType::Consumer, 50),
       PowerSystem(3, PowerSystemType::Bus),
       PowerSystem(4, PowerSystemType::Bus),
       PowerSystem(5, PowerSystemType::Bus),
       PowerSystem(6, PowerSystemType::Bus),
       PowerSystem(7, PowerSystemType::Bus),
       PowerSystem(8, PowerSystemType::Generator, 50),
       PowerSystem(9, PowerSystemType::Generator, 30),
       PowerSystem(10, PowerSystemType::Generator, 30),
       },
      CircuitBreakerVectorFactory()
          .addCircuitBreaker(0, 3, 1000, 5)
          .addCircuitBreaker(3, 6, 1000, 5)
          .addCircuitBreaker(6, 10, 1000, 5)
          .addCircuitBreaker(3, 7, 1000, 5)
          .addCircuitBreaker(7, 9, 1000, 5)
          .addCircuitBreaker(7, 4, 1000, 5)
          .addCircuitBreaker(4, 1, 1000, 5)
          .addCircuitBreaker(4, 5, 1000, 5)
          .addCircuitBreaker(2, 5, 1000, 5)
          .addCircuitBreaker(8, 5, 1000, 5)
          .toVector(),
  };
  StateExplorer stateExplorer(&grid, State(&grid));
  debug(grid.toString());
  stateExplorer.generateStateSpace();
  // stateExplorer.prettyPrint();
  stateExplorer.prettyPrintBestStateTrace();
  std::cout << "State Space Size: " << stateExplorer.stateSpace.size() << "\n";
}
