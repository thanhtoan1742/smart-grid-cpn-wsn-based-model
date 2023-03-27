#include "CircuitBreaker.h"

CircuitBreaker::CircuitBreaker(
    int _id, int _inp, int _out, int _capacity, int _loss
)
    : id{_id}, inp{_inp}, out{_out}, capacity{_capacity}, loss{_loss} {
}
