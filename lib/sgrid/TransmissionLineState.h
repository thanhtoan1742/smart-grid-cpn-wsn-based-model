#ifndef SGRID_TRANSMISSION_LINE_STATE_H
#define SGRID_TRANSMISSION_LINE_STATE_H

#include <sgrid/Power.h>
#include <sgrid/TransmissionLine.h>

namespace sgrid {
struct TransmissionLineState {
  TransmissionLine* tl;
  Power             used;

  TransmissionLineState(TransmissionLine* tl, Power used = 0);

  bool operator==(TransmissionLineState const& other) const&;

  void  transmit(Power amount);
  Power transmittable() const&;

  std::string toString() const&;
};
} // namespace sgrid

#endif // SGRID_TRANSMISSION_LINE_STATE_H
