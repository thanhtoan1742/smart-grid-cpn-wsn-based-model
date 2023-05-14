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
  state.psStates[idx]->keeping = keeping;
  state.psStates[idx]->used    = used;
}

TEST(MinFulfilled, Test1) {
  /*
   *    C0---10%---G1
   *
   * */
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 1, Power::maxPower, 0.1)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 11;

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Test2) {
  /*
   *    C0---10%---B1---10%---G2
   *
   * */
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Bus)
                  .createPowerSystem(2, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 1, Power::maxPower, 0.1)
                  .createTransmissionLine(1, 2, Power::maxPower, 0.1)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got = stateExplorer.minFulfilled;
  Power expect(12.1);

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Test3) {
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
                  .createTransmissionLine(0, 3, Power::maxPower, 0.05)
                  .createTransmissionLine(3, 6, Power::maxPower, 0.05)
                  .createTransmissionLine(6, 10, Power::maxPower, 0.05)
                  .createTransmissionLine(3, 7, Power::maxPower, 0.05)
                  .createTransmissionLine(7, 9, Power::maxPower, 0.05)
                  .createTransmissionLine(7, 4, Power::maxPower, 0.05)
                  .createTransmissionLine(4, 1, Power::maxPower, 0.05)
                  .createTransmissionLine(4, 5, Power::maxPower, 0.05)
                  .createTransmissionLine(2, 5, Power::maxPower, 0.05)
                  .createTransmissionLine(8, 5, Power::maxPower, 0.05)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  // stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 173;

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Test4) {
  /*
   *    C0----30%---G2
   *
   *    C1----10%---G3
   *
   *    min = 24
   */
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Consumer, 10)
                  .createPowerSystem(2, PowerSystemType::Generator, 20)
                  .createPowerSystem(3, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 2, Power::maxPower, 0.1)
                  .createTransmissionLine(1, 3, Power::maxPower, 0.3)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 24;

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Test5) {
  /*
   *    G20 -- 20 -- B -- 10 -- B -- 0 -- C10
   *                 |          |
   *                 10         10
   *                 |          |
   *    G20 -- 10 -- B -- 30 -- B -- 0 -- C10
   */
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Consumer, 10)
                  .createPowerSystem(2, PowerSystemType::Bus)
                  .createPowerSystem(3, PowerSystemType::Bus)
                  .createPowerSystem(4, PowerSystemType::Bus)
                  .createPowerSystem(5, PowerSystemType::Bus)
                  .createPowerSystem(6, PowerSystemType::Generator, 20)
                  .createPowerSystem(7, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 3, Power::maxPower, 0)
                  .createTransmissionLine(1, 2, Power::maxPower, 0)
                  .createTransmissionLine(2, 3, Power::maxPower, 0.1)
                  .createTransmissionLine(3, 4, Power::maxPower, 0.1)
                  .createTransmissionLine(2, 5, Power::maxPower, 0.3)
                  .createTransmissionLine(4, 5, Power::maxPower, 0.1)
                  .createTransmissionLine(4, 7, Power::maxPower, 0.2)
                  .createTransmissionLine(5, 6, Power::maxPower, 0.1)
                  .createGrid();

  StateExplorer stateExplorer(&grid, State(&grid));
  // stateExplorer.generateStateSpace();
  Power got = stateExplorer.minFulfilled;
  Power expect(27.5);

  EXPECT_EQ(got, expect);
}

TEST(MinFulfilled, Test6) {
  /*
   * 	              /-----3%----G15
   *   C20----2%----B
   *   	            \-----5%----G15
   *
   */
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 20)
                  .createPowerSystem(1, PowerSystemType::Bus)
                  .createPowerSystem(2, PowerSystemType::Generator, 15)
                  .createPowerSystem(3, PowerSystemType::Generator, 15)
                  .createTransmissionLine(0, 1, Power::maxPower, 0.02)
                  .createTransmissionLine(1, 2, Power::maxPower, 0.05)
                  .createTransmissionLine(1, 3, Power::maxPower, 0.03)
                  .createGrid();

  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got = stateExplorer.minFulfilled;
  Power expect(21.1287);

  EXPECT_EQ(got, expect);
}
} // namespace sgrid
