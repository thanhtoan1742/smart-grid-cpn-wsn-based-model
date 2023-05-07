#ifndef SGRID_CARRIER_STATE_H
#define SGRID_CARRIER_STATE_H

#include <sgrid/Percentage.h>
#include <sgrid/Power.h>
#include <sgrid/PowerSystem.h>

#include <functional>

namespace sgrid {

struct PowerSystemState {
  PowerSystem* ps;
  Power        keeping;
  Power        used;

  PowerSystemState(PowerSystem* ps, Power keeping = 0, Power used = 0);

  bool operator==(PowerSystemState const& other) const&;

  void fulfill();
  PowerSystemState fulfilled() const&;
  PowerSystemState sent(Power amount) const&;
  PowerSystemState received(Power amount) const&;

  Power fulfillable() const&;

  std::string toString() const&;
};

} // namespace sgrid
#endif // SGRID_CARRIER_STATE_H
