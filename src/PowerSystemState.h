#ifndef CARRIER_STATE_H
#define CARRIER_STATE_H

#include <functional>

#include "Percentage.h"
#include "Power.h"
#include "PowerSystem.h"

struct PowerSystemState {
  static Power const maxKeeping;

  PowerSystem* car;
  Power        keeping;
  Power        used;

  PowerSystemState(PowerSystem* car, Power keeping = 0, Power used = 0);

  bool operator==(PowerSystemState const& other) const&;

  PowerSystemState fulfill() const&;
  PowerSystemState demand() const&;
  PowerSystemState send(Power amount) const&;
  PowerSystemState receive(Power amount) const&;

  std::string toString() const&;
};

#endif // CARRIER_STATE_H
