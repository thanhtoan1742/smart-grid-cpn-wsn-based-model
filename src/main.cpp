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
  //      TransmissionLine(0, 0, 3, Power::maxPower, 0),
  //      TransmissionLine(1, 3, 0, Power::maxPower, 0),
  //      TransmissionLine(2, 1, 2, Power::maxPower, 0),
  //      TransmissionLine(3, 2, 1, Power::maxPower, 0),
  //      TransmissionLine(4, 3, 2, Power::maxPower, 10),
  //      TransmissionLine(5, 2, 3, Power::maxPower, 10),
  //      TransmissionLine(6, 3, 4, Power::maxPower, 10),
  //      TransmissionLine(7, 4, 3, Power::maxPower, 10),
  //      TransmissionLine(8, 2, 5, Power::maxPower, 30),
  //      TransmissionLine(9, 5, 2, Power::maxPower, 30),
  //      TransmissionLine(10, 4, 5, Power::maxPower, 10),
  //      TransmissionLine(11, 5, 4, Power::maxPower, 10),
  //      TransmissionLine(12, 5, 6, Power::maxPower, 10),
  //      TransmissionLine(13, 6, 5, Power::maxPower, 10),
  //      TransmissionLine(14, 4, 7, Power::maxPower, 20),
  //      TransmissionLine(15, 7, 4, Power::maxPower, 20),
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
    .createPowerSystem(0, PowerSystemType::Consumer, 50)
    .createPowerSystem(1, PowerSystemType::Consumer, 50)
    .createPowerSystem(2, PowerSystemType::Consumer, 50)
    .createPowerSystem(3, PowerSystemType::Bus)
    .createPowerSystem(4, PowerSystemType::Bus)
    .createPowerSystem(5, PowerSystemType::Bus)
    .createPowerSystem(6, PowerSystemType::Bus)
    .createPowerSystem(7, PowerSystemType::Bus)
    .createPowerSystem(8, PowerSystemType::Generator, 50)
    .createPowerSystem(9, PowerSystemType::Generator, 120)
    .createPowerSystem(10, PowerSystemType::Generator, 30)
    .createTransmissionLine(0, 3, Power::maxPower, 5)
    .createTransmissionLine(3, 6, Power::maxPower, 5)
    .createTransmissionLine(6, 10, Power::maxPower, 5)
    .createTransmissionLine(3, 7, Power::maxPower, 5)
    .createTransmissionLine(7, 9, Power::maxPower, 5)
    .createTransmissionLine(7, 4, Power::maxPower, 5)
    .createTransmissionLine(4, 1, Power::maxPower, 5)
    .createTransmissionLine(4, 5, Power::maxPower, 5)
    .createTransmissionLine(2, 5, Power::maxPower, 5)
    .createTransmissionLine(8, 5, Power::maxPower, 5)
    .createGrid();
  // clang-format on

  StateExplorer stateExplorer(&grid, State(&grid));
  debug(grid.toString());
  // stateExplorer.generateStateSpace();
  // // stateExplorer.prettyPrint();
  // stateExplorer.prettyPrintBestStateTrace();
  // std::cout << "State Space Size: " << stateExplorer.stateSpace.size() <<
  // "\n";
}
