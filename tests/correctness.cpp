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

TEST(Correctness, SimpleOneOnOne) {

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

TEST(Correctness, Simple3Nodes) {
  // Grid grid = Grid{
  //     {
  //      PowerSystem(0, PowerSystemType::Consumer, 10),
  //      PowerSystem(1, PowerSystemType::Bus),
  //      PowerSystem(2, PowerSystemType::Generator, 20),
  //      },
  //     {
  //      TransmissionLine(0, 0, 1, Power::maxPower, 10),
  //      TransmissionLine(1, 1, 2, Power::maxPower, 10),
  //      }
  // };
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

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

} // namespace sgrid
