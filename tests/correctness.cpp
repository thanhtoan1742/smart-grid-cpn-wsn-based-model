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
  State& got = *stateExplorer.bestState;

  State expect = State(got);
  setKU(expect, 0, 0, 10);
  setKU(expect, 1, 0, 11);

  EXPECT_EQ(got, expect) << fmt::format(
      "\ngot    = {}\nexpect = {}",
      got.toString(),
      expect.toString()
  );
}

TEST(Correctness, Simple3Nodes) {

  Grid grid = Grid{
      {
       PowerSystem(0, PowerSystemType::Consumer, 10),
       PowerSystem(1, PowerSystemType::Bus),
       PowerSystem(2, PowerSystemType::Generator, 20),
       },
      {
       TransmissionLine(0, 0, 1, Power::maxPower, 10),
       TransmissionLine(1, 1, 2, Power::maxPower, 10),
       }
  };
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  State& got = *stateExplorer.bestState;

  State expect = State(got);
  setKU(expect, 0, 0, 10);
  setKU(expect, 1, 0, 0);
  setKU(expect, 2, 0, 12);

  EXPECT_EQ(got, expect) << fmt::format(
      "\ngot    = {}\nexpect = {}",
      got.toString(),
      expect.toString()
  );
}

TEST(Correctness, Complex) {
  auto grid = Grid{
      {
       PowerSystem(0, PowerSystemType::Consumer, 10),
       PowerSystem(1, PowerSystemType::Consumer, 10),
       PowerSystem(2, PowerSystemType::Bus),
       PowerSystem(3, PowerSystemType::Bus),
       PowerSystem(4, PowerSystemType::Bus),
       PowerSystem(5, PowerSystemType::Bus),
       PowerSystem(6, PowerSystemType::Generator, 50),
       PowerSystem(7, PowerSystemType::Generator, 50),
       },
      {
       TransmissionLine(0, 0, 3, Power::maxPower, 0),
       TransmissionLine(1, 3, 0, Power::maxPower, 0),
       TransmissionLine(2, 1, 2, Power::maxPower, 0),
       TransmissionLine(3, 2, 1, Power::maxPower, 0),
       TransmissionLine(4, 3, 2, Power::maxPower, 10),
       TransmissionLine(5, 2, 3, Power::maxPower, 10),
       TransmissionLine(6, 3, 4, Power::maxPower, 10),
       TransmissionLine(7, 4, 3, Power::maxPower, 10),
       TransmissionLine(8, 2, 5, Power::maxPower, 30),
       TransmissionLine(9, 5, 2, Power::maxPower, 30),
       TransmissionLine(10, 4, 5, Power::maxPower, 10),
       TransmissionLine(11, 5, 4, Power::maxPower, 10),
       TransmissionLine(12, 5, 6, Power::maxPower, 10),
       TransmissionLine(13, 6, 5, Power::maxPower, 10),
       TransmissionLine(14, 4, 7, Power::maxPower, 20),
       TransmissionLine(15, 7, 4, Power::maxPower, 20),
       }
  };

  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  State& got = *stateExplorer.bestState;

  State expect = State(got);
  setKU(expect, 0, 0, 10);
  setKU(expect, 1, 0, 10);
  setKU(expect, 2, 0, 0);
  setKU(expect, 3, 0, 0);
  setKU(expect, 4, 0, 0);
  setKU(expect, 5, 0, 0);
  setKU(expect, 6, 0, 0);
  setKU(expect, 7, 0, 29);

  EXPECT_EQ(got, expect) << fmt::format(
      "\ngot    = {}\nexpect = {}",
      got.toString(),
      expect.toString()
  );
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

} // namespace sgrid
