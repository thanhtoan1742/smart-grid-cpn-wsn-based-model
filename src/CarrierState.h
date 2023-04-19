#ifndef CARRIER_STATE_H
#define CARRIER_STATE_H

#include <functional>

#include "Percentage.h"
#include "Power.h"
#include "PowerSystem.h"

struct CarrierState {
  static Power const maxKeeping;

  PowerSystem* car;
  Power        keeping;
  Power        used;

  CarrierState(PowerSystem* car, Power keeping = 0, Power used = 0);

  bool operator==(CarrierState const& other) const&;

  CarrierState fulfill() const&;
  CarrierState demand() const&;
  CarrierState send(Power amount) const&;
  CarrierState receive(Power amount) const&;

  std::string toString() const&;
};

#endif // CARRIER_STATE_H
