#ifndef CARRIER_STATE_H
#define CARRIER_STATE_H

#include <functional>

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>

namespace sgrid {

struct PowerSystemState {
  PowerSystem* ps;
  Power        keeping;
  Power        used;
  Power        remainCap;

  PowerSystemState(PowerSystem* ps, Power keeping = 0, Power used = 0);

  bool operator==(PowerSystemState const& other) const&;

  PowerSystemState fulfill() const&;
  PowerSystemState send(Power amount) const&;
  PowerSystemState receive(Power amount) const&;

  std::string toString() const&;
};

} // namespace sgrid
#endif // CARRIER_STATE_H
