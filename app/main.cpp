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
  plog::init(plog::info, &consoleAppender).addAppender(&fileAppender);
  PLOGI << "APP STARTED";
}

int main() {
  initLogger();

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
  PLOGI << "\n" << stateExplorer.stateSpaceToString();
  PLOGI << "\nBETS STATE STACK TRACE";
  PLOGI << "\n" << stateExplorer.bestStateTraceToString();
  PLOGI << "Minimum Fulfilled: " << stateExplorer.minFulfilled;
  PLOGI << "State Space Size: " << stateExplorer.stateSpace.size();
}
