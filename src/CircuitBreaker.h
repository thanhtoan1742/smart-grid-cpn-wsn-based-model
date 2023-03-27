#ifndef CIRCUIT_BREAKER_H
#define CIRCUIT_BREAKER_H

struct CircuitBreaker {
  const int id;
  const int inp;
  const int out;
  const int capacity;
  const int loss;

  CircuitBreaker(int id, int inp, int out, int capacit = 0, int loss = 0);
};

#endif // CIRCUIT_BREAKER_H
