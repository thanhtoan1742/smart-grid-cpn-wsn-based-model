#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include "Grid.h"
#include "Percentage.h"
#include "Power.h"
#include "PowerSystem.h"
#include "PowerSystemState.h"
#include "State.h"
#include "StateExplorer.h"
#include "TransmissionLine.h"
#include "utils.h"

using namespace sgrid;

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
  //      TransmissionLine(0, 0, 3, 1000, 0),
  //      TransmissionLine(1, 3, 0, 1000, 0),
  //      TransmissionLine(2, 1, 2, 1000, 0),
  //      TransmissionLine(3, 2, 1, 1000, 0),
  //      TransmissionLine(4, 3, 2, 1000, 10),
  //      TransmissionLine(5, 2, 3, 1000, 10),
  //      TransmissionLine(6, 3, 4, 1000, 10),
  //      TransmissionLine(7, 4, 3, 1000, 10),
  //      TransmissionLine(8, 2, 5, 1000, 30),
  //      TransmissionLine(9, 5, 2, 1000, 30),
  //      TransmissionLine(10, 4, 5, 1000, 10),
  //      TransmissionLine(11, 5, 4, 1000, 10),
  //      TransmissionLine(12, 5, 6, 1000, 10),
  //      TransmissionLine(13, 6, 5, 1000, 10),
  //      TransmissionLine(14, 4, 7, 1000, 20),
  //      TransmissionLine(15, 7, 4, 1000, 20),
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
  // clang-format off
  Grid grid = GridFactory()
    .createPowerSystem(PowerSystemType::Consumer, 50)
    .createPowerSystem(PowerSystemType::Consumer, 50)
    .createPowerSystem(PowerSystemType::Consumer, 50)
    .createPowerSystem(PowerSystemType::Bus)
    .createPowerSystem(PowerSystemType::Bus)
    .createPowerSystem(PowerSystemType::Bus)
    .createPowerSystem(PowerSystemType::Bus)
    .createPowerSystem(PowerSystemType::Bus)
    .createPowerSystem(PowerSystemType::Generator, 50)
    .createPowerSystem(PowerSystemType::Generator, 120)
    .createPowerSystem(PowerSystemType::Generator, 30)
    .createTransmissionLine(0, 3, 1000, 5)
    .createTransmissionLine(3, 6, 1000, 5)
    .createTransmissionLine(6, 10, 1000, 5)
    .createTransmissionLine(3, 7, 1000, 5)
    .createTransmissionLine(7, 9, 1000, 5)
    .createTransmissionLine(7, 4, 1000, 5)
    .createTransmissionLine(4, 1, 1000, 5)
    .createTransmissionLine(4, 5, 1000, 5)
    .createTransmissionLine(2, 5, 1000, 5)
    .createTransmissionLine(8, 5, 1000, 5)
    .createGrid();
  // clang-format on

  StateExplorer stateExplorer(&grid, State(&grid));
  debug(grid.toString());
  stateExplorer.generateStateSpace();
  // stateExplorer.prettyPrint();
  stateExplorer.prettyPrintBestStateTrace();
  std::cout << "State Space Size: " << stateExplorer.stateSpace.size() << "\n";
}
