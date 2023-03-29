#include "CircuitBreaker.h"
#include "Percentage.h"
#include "Power.h"

CircuitBreaker::CircuitBreaker(
    int _id, int _inp, int _out, Power _capacity, Percentage _loss
)
    : id{_id}, inp{_inp}, out{_out}, capacity{_capacity}, loss{_loss} {
}
