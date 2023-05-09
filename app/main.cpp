#include <sgrid/Grid.h>
#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>
#include <sgrid/PowerSystemState.h>
#include <sgrid/State.h>
#include <sgrid/StateExplorer.h>
#include <sgrid/TransmissionLine.h>
#include <sgrid/utils.h>

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace sgrid;

void initLogger() {
  char const* filename = "test.log";

  std::ofstream ofs(filename);
  ofs << "";
  ofs.close();
  static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(
      filename,
      512 << 20,
      5
  );
  static plog::ColorConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::debug, &consoleAppender).addAppender(&fileAppender);
  PLOGI << "APP STARTED";
}

int main() {
  initLogger();

  // clang-format off
  /*
   *    C0----0-----B3----10----B4----20----G7
   *                |           |
   *                10          10
   *                |           |
   *    C1----0-----B2----30----B5----10----G6
   */
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

  // Grid grid = Grid{
  //     {
  //      PowerSystem(0, PowerSystemType::Consumer, 10),
  //      PowerSystem(1, PowerSystemType::Generator, 20),
  //      },
  //     {
  //      TransmissionLine(0, 0, 1, Power::maxPower, 10),
  //      }
  // };

  // Grid grid = GridFactory()
  //   .createPowerSystem(0, PowerSystemType::Consumer, 50)
  //   .createPowerSystem(1, PowerSystemType::Consumer, 50)
  //   .createPowerSystem(2, PowerSystemType::Consumer, 50)
  //   .createPowerSystem(3, PowerSystemType::Bus)
  //   .createPowerSystem(4, PowerSystemType::Bus)
  //   .createPowerSystem(5, PowerSystemType::Bus)
  //   .createPowerSystem(6, PowerSystemType::Bus)
  //   .createPowerSystem(7, PowerSystemType::Bus)
  //   .createPowerSystem(8, PowerSystemType::Generator, 150)
  //   .createPowerSystem(9, PowerSystemType::Generator, 120)
  //   .createPowerSystem(10, PowerSystemType::Generator, 30)
  //   .createTransmissionLine(0, 3, Power::maxPower, 5)
  //   .createTransmissionLine(3, 6, Power::maxPower, 5)
  //   .createTransmissionLine(6, 10, Power::maxPower, 5)
  //   .createTransmissionLine(3, 7, Power::maxPower, 5)
  //   .createTransmissionLine(7, 9, Power::maxPower, 5)
  //   .createTransmissionLine(7, 4, Power::maxPower, 5)
  //   .createTransmissionLine(4, 1, Power::maxPower, 5)
  //   .createTransmissionLine(4, 5, Power::maxPower, 5)
  //   .createTransmissionLine(2, 5, Power::maxPower, 5)
  //   .createTransmissionLine(8, 5, Power::maxPower, 5)
  //   .createGrid();

  // Grid grid = Grid{
  //     {
  //      PowerSystem(0, PowerSystemType::Consumer, 10),
  //      PowerSystem(1, PowerSystemType::Generator, 20),
  //      },
  //     {
  //      TransmissionLine(0, 0, 1, Power::maxPower, 10),
  //      }
  // };


  // Grid grid = GridFactory()
  //   .createPowerSystem(0, PowerSystemType::Consumer, 30)
  //   .createPowerSystem(1, PowerSystemType::Bus)
  //   .createPowerSystem(2, PowerSystemType::Bus)
  //   .createPowerSystem(3, PowerSystemType::Generator, 50)
  //   .createTransmissionLine(0, 1, Power::maxPower, 5)
  //   .createTransmissionLine(1, 2, Power::maxPower, 5)
  //   .createTransmissionLine(2, 3, Power::maxPower, 5)
  //   .createGrid();




  // Grid grid = GridFactory()
  //   .createPowerSystem(0, PowerSystemType::Consumer, 20)
  //   .createPowerSystem(1, PowerSystemType::Bus)
  //   .createPowerSystem(2, PowerSystemType::Generator, 30)
  //   .createPowerSystem(3, PowerSystemType::Generator, 30)
  //   .createTransmissionLine(0, 1, Power::maxPower, 2)
  //   .createTransmissionLine(1, 2, Power::maxPower, 5)
  //   .createTransmissionLine(1, 3, Power::maxPower, 3)
  //   .createGrid();


  // Grid grid = GridFactory()
  //   .createPowerSystem(0, PowerSystemType::Consumer, 20)
  //   .createPowerSystem(1, PowerSystemType::Bus)
  //   .createPowerSystem(2, PowerSystemType::Generator, 15)
  //   .createPowerSystem(3, PowerSystemType::Generator, 15)
  //   .createTransmissionLine(0, 1, Power::maxPower, 2)
  //   .createTransmissionLine(1, 2, Power::maxPower, 5)
  //   .createTransmissionLine(1, 3, Power::maxPower, 3)
  //   .createGrid();


  /*
   *    G20 -- 20 -- B -- 10 -- B -- 0 -- C10
   *                 |          |
   *                 10         20
   *                 |          |
   *    G20 -- 10 -- B -- 30 -- B -- 0 -- C10
  */
  // Grid grid = GridFactory()
  //   .createPowerSystem(0, PowerSystemType::Consumer, 10)
  //   .createPowerSystem(1, PowerSystemType::Consumer, 10)
  //   .createPowerSystem(2, PowerSystemType::Bus)
  //   .createPowerSystem(3, PowerSystemType::Bus)
  //   .createPowerSystem(4, PowerSystemType::Bus)
  //   .createPowerSystem(5, PowerSystemType::Bus)
  //   .createPowerSystem(6, PowerSystemType::Generator, 20)
  //   .createPowerSystem(7, PowerSystemType::Generator, 20)
  //   .createTransmissionLine(0, 3, Power::maxPower, 0)
  //   .createTransmissionLine(1, 2, Power::maxPower, 0)
  //   .createTransmissionLine(2, 3, Power::maxPower, 10)
  //   .createTransmissionLine(3, 4, Power::maxPower, 10)
  //   .createTransmissionLine(2, 5, Power::maxPower, 30)
  //   .createTransmissionLine(4, 5, Power::maxPower, 10)
  //   .createTransmissionLine(4, 7, Power::maxPower, 20)
  //   .createTransmissionLine(5, 6, Power::maxPower, 10)
  //   .createGrid();


  /*
   *    C0----30---G2
   *
   *    C1----10---G3
   */
  // Grid grid = GridFactory()
  //   .createPowerSystem(0, PowerSystemType::Consumer, 10)
  //   .createPowerSystem(1, PowerSystemType::Consumer, 10)
  //   .createPowerSystem(2, PowerSystemType::Generator, 20)
  //   .createPowerSystem(3, PowerSystemType::Generator, 20)
  //   .createTransmissionLine(0, 2, Power::maxPower, 10)
  //   .createTransmissionLine(1, 3, Power::maxPower, 30)
  //   .createGrid();
  // clang-format on

  PLOGD << "\n" << grid.toString();

  State state = State(&grid);
  // state.calculateOutcomes();
  // PLOGD << "OUTCOME";
  // for (auto outcome: state.outcomes) {
  //   if (outcome)
  //     PLOGD << outcome->toString();
  //   else
  //     PLOGD << outcome;
  // }

  StateExplorer stateExplorer(&grid, state);
  stateExplorer.generateStateSpace();
  // PLOGD << "\n" << stateExplorer.stateSpaceToString();
  PLOGD << "\n" << stateExplorer.bestStateTraceToString();
  PLOGD << "Minimum Fulfilled: " << stateExplorer.minFulfilled;
  PLOGD << "State Space Size: " << stateExplorer.stateSpace.size();
}
