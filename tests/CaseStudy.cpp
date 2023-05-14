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

TEST(CaseStudy, Size5) {
  /*
   * 	              /-----10:1%----B------2%-----G15
   *   C20----2%----B
   *   	            \-----5%----G15
   *
   */
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Consumer, 20)
                  .createPowerSystem(1, PowerSystemType::Bus)
                  .createPowerSystem(2, PowerSystemType::Bus)
                  .createPowerSystem(3, PowerSystemType::Generator, 15)
                  .createPowerSystem(4, PowerSystemType::Generator, 15)
                  .createTransmissionLine(0, 1, Power::maxPower, 0.02)
                  .createTransmissionLine(1, 2, 10, 0.01)
                  .createTransmissionLine(2, 3, Power::maxPower, 0.02)
                  .createTransmissionLine(1, 4, Power::maxPower, 0.05)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 21.222;

  EXPECT_EQ(got, expect);
}

TEST(CaseStudy, Size10) {
  /*
   *    C0---------1%---------------B-----20:2%------G9
   *                                |
   *                               3%
   *                                |
   *    C1----2%-----B4------1%----B6-------5%----G8
   *                 |
   *                2%
   *                 |
   *    C2-----4%----B5------40:1%----G7
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
                  .createPowerSystem(7, PowerSystemType::Generator, 50)
                  .createPowerSystem(8, PowerSystemType::Generator, 150)
                  .createPowerSystem(9, PowerSystemType::Generator, 30)
                  .createTransmissionLine(0, 3, Power::maxPower, 0.01)
                  .createTransmissionLine(3, 9, 20, 0.02)
                  .createTransmissionLine(3, 6, Power::maxPower, 0.03)
                  .createTransmissionLine(1, 4, Power::maxPower, 0.02)
                  .createTransmissionLine(4, 6, Power::maxPower, 0.01)
                  .createTransmissionLine(6, 8, Power::maxPower, 0.05)
                  .createTransmissionLine(4, 5, Power::maxPower, 0.02)
                  .createTransmissionLine(2, 5, Power::maxPower, 0.04)
                  .createTransmissionLine(5, 7, 40, 0.01)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 160.852;

  EXPECT_EQ(got, expect);
}

TEST(CaseStudy, Size25) {
  Grid grid = GridFactory()
                  .createPowerSystem(0, PowerSystemType::Generator, 1500)
                  .createPowerSystem(1, PowerSystemType::Generator, 6000)
                  .createPowerSystem(2, PowerSystemType::Generator, 1800)
                  .createPowerSystem(3, PowerSystemType::Generator, 2700)
                  .createPowerSystem(4, PowerSystemType::Consumer, 200)
                  .createPowerSystem(5, PowerSystemType::Consumer, 290)
                  .createPowerSystem(6, PowerSystemType::Consumer, 710)
                  .createPowerSystem(7, PowerSystemType::Consumer, 530)
                  .createPowerSystem(8, PowerSystemType::Consumer, 480)
                  .createPowerSystem(9, PowerSystemType::Consumer, 120)
                  .createPowerSystem(10, PowerSystemType::Consumer, 710)
                  .createPowerSystem(11, PowerSystemType::Bus)
                  .createPowerSystem(12, PowerSystemType::Bus)
                  .createPowerSystem(13, PowerSystemType::Bus)
                  .createPowerSystem(14, PowerSystemType::Bus)
                  .createPowerSystem(15, PowerSystemType::Bus)
                  .createPowerSystem(16, PowerSystemType::Bus)
                  .createPowerSystem(17, PowerSystemType::Bus)
                  .createPowerSystem(18, PowerSystemType::Bus)
                  .createPowerSystem(19, PowerSystemType::Bus)
                  .createPowerSystem(20, PowerSystemType::Bus)
                  .createPowerSystem(21, PowerSystemType::Bus)
                  .createPowerSystem(22, PowerSystemType::Bus)
                  .createPowerSystem(23, PowerSystemType::Bus)
                  .createPowerSystem(24, PowerSystemType::Bus)
                  .createTransmissionLine(11, 3, 500, 0.01)
                  .createTransmissionLine(12, 2, 600, 0.01)
                  .createTransmissionLine(13, 1, 1700, 0.04)
                  .createTransmissionLine(14, 1, 1200, 0.0)
                  .createTransmissionLine(15, 3, 2000, 0.03)
                  .createTransmissionLine(16, 0, 800, 0.01)
                  .createTransmissionLine(17, 2, 1800, 0.02)
                  .createTransmissionLine(18, 1, 1900, 0.06)
                  .createTransmissionLine(19, 0, 1200, 0.03)
                  .createTransmissionLine(20, 0, 1400, 0.01)
                  .createTransmissionLine(21, 0, 1700, 0.02)
                  .createTransmissionLine(22, 3, 2000, 0.01)
                  .createTransmissionLine(23, 3, 1800, 0.01)
                  .createTransmissionLine(24, 1, 1400, 0.03)
                  .createTransmissionLine(0, 16, 600, 0.03)
                  .createTransmissionLine(1, 22, 1600, 0.01)
                  .createTransmissionLine(2, 14, 1300, 0.01)
                  .createTransmissionLine(3, 12, 1300, 0.0)
                  .createTransmissionLine(4, 12, 600, 0.04)
                  .createTransmissionLine(5, 13, 600, 0.06)
                  .createTransmissionLine(6, 20, 600, 0.02)
                  .createTransmissionLine(7, 23, 800, 0.05)
                  .createTransmissionLine(8, 12, 1400, 0.01)
                  .createTransmissionLine(9, 21, 1900, 0.01)
                  .createTransmissionLine(10, 22, 1800, 0.02)
                  .createTransmissionLine(19, 12, 500, 0.06)
                  .createTransmissionLine(11, 6, 2000, 0.01)
                  .createTransmissionLine(0, 16, 1500, 0.0)
                  .createTransmissionLine(15, 12, 1500, 0.01)
                  .createTransmissionLine(18, 3, 1400, 0.06)
                  .createTransmissionLine(20, 13, 1900, 0.0)
                  .createTransmissionLine(21, 8, 2000, 0.06)
                  .createTransmissionLine(12, 16, 700, 0.0)
                  .createGrid();
  StateExplorer stateExplorer(&grid, State(&grid));
  stateExplorer.generateStateSpace();
  Power got    = stateExplorer.minFulfilled;
  Power expect = 3146.85;

  EXPECT_EQ(got, expect);
}

} // namespace sgrid
