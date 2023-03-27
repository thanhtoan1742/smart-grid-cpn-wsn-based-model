#ifndef CIRCUIT_BREAKER_H
#define CIRCUIT_BREAKER_H

struct CircuitBreaker {
  int const id;
  int const inp;
  int const out;
  int const capacity;
  int const loss;

  CircuitBreaker(int id, int inp, int out, int capacit = 0, int loss = 0);
};

#endif // CIRCUIT_BREAKER_H
