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
#include "plog/Severity.h"
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
                  .createPowerSystem(0, PowerSystemType::Consumer, 20)
                  .createPowerSystem(1, PowerSystemType::Bus)
                  .createPowerSystem(2, PowerSystemType::Bus)
                  .createPowerSystem(3, PowerSystemType::Generator, 15)
                  .createPowerSystem(4, PowerSystemType::Generator, 15)
                  .createTransmissionLine(0, 1, Power::maxPower, 0.02)
                  .createTransmissionLine(1, 2, Power::maxPower, 0.01)
                  .createTransmissionLine(2, 3, Power::maxPower, 0.02)
                  .createTransmissionLine(1, 4, Power::maxPower, 0.05)
                  .createGrid();

  PLOGD << "\n" << grid.toString();

  State state = State(&grid);
  state.calculateOutcomes();
  PLOGD << "OUTCOME";
  for (auto outcome: state.outcomes) {
    if (outcome)
      PLOGD << outcome->toString() << " " << outcome->fulfillable();
    else
      PLOGD << outcome;
  }

  StateExplorer stateExplorer(&grid, state);
  stateExplorer.generateStateSpace();
  // PLOGI << "\n" << stateExplorer.stateSpaceToString();
  PLOGI << "\nBETS STATE STACK TRACE";
  PLOGI << "\n" << stateExplorer.bestStateTraceToString();
  PLOGI << "Minimum Fulfilled: " << stateExplorer.minFulfilled;
  PLOGI << "State Space Size: " << stateExplorer.stateSpace.size();
}
