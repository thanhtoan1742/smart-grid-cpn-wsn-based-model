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

TEST(TransmissionLine, Simple) {
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 10)
                  .createPowerSystem(1, PowerSystemType::Generator, 20)
                  .createPowerSystem(2, PowerSystemType::Generator, 20)
                  .createTransmissionLine(0, 1, 5, 0)
                  .createTransmissionLine(0, 2, 10, 100)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 15;

  EXPECT_EQ(got, expect);
}

} // namespace sgrid
