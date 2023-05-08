#include <iostream>
#include <new>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <gtest/gtest.h>

#include <sgrid/Grid.h>
#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/State.h>
#include <sgrid/StateExplorer.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

namespace sgrid {

void setKU(State& state, i32 idx, Power keeping, Power used) {
  state.psStates[idx].keeping = keeping;
  state.psStates[idx].used    = used;
}

TEST(MinFulfilled, SimpleOneOnOne) {

  // Grid grid = Grid{
  //     {
  //      PowerSystem(0, PowerSystemType::Consumer, 10),
  //      PowerSystem(1, PowerSystemType::Generator, 20),
  //      },
  //     {
  //      TransmissionLine(0, 0, 1, Power::maxPower, 10),
  //      }
  // };
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 1, Power::maxPower, 10)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 11;

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Simple3Nodes) {

  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Bus)
                  .createPowerSystem(2, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 1, Power::maxPower, 10)
                  .createTransmissionLine(1, 2, Power::maxPower, 10)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 13;

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Complex) {
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
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 173;

  EXPECT_EQ(got, expect);
}

} // namespace sgrid
