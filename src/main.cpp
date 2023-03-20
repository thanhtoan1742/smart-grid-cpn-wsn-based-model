#include <iostream>
#include <iterator>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>


using namespace std;

enum CarrierType { CTGenerator = 'G', CTConsumer = 'C', CTBus = 'B' };

struct Carrier {
  const int id;
  const CarrierType ct;
  const int capacity;
  int keeping;
  int used;

  Carrier(int _id, CarrierType _ct, int _capacity = 0, int _keeping = 0,
          int _used = 0)
      : id(_id), ct(_ct), capacity(_capacity), keeping(_keeping), used(_used) {}

  string toString() const & {
    return string(1, ct) + to_string(id) + ":" + to_string(keeping) + ":" +
           to_string(used);
  }
};

struct CircuitBreaker {
  const int id;
  const int inp;
  const int out;
  const int capacity;
  const int loss;

  CircuitBreaker(int _id, int _inp, int _out, int _capacity = 0, int _loss = 0)
      : id(_id), inp(_inp), out(_out), capacity(_capacity), loss(_loss) {}
};

int withLoss(int amount, int loss) {
  // floor(amount * (100 + loss)/100)
  return (amount * (100 + loss) + 99) / 100;
}

struct State {
  vector<Carrier> cars;
  vector<CircuitBreaker> cbs;
  int depth;

  State() : depth(0) {}
  State(vector<Carrier> const &_cars, vector<CircuitBreaker> const &_cbs,
        int _depth = 0)
      : cars(_cars), cbs(_cbs), depth(_depth) {}
  State(State const &original)
      : cars(original.cars), cbs(original.cbs), depth(original.depth) {}

  State demand(int idx) const & {
    State ns(*this);
    Carrier &car = ns.cars[idx];
    int amount = car.capacity - car.used;
    car.keeping += amount;
    car.used += amount;
    return ns;
  }

  State fulfill(int idx) const & {
    State ns(*this);
    Carrier &car = ns.cars[idx];
    int amount = max(0, min(car.keeping, car.capacity - car.used));
    car.used += amount;
    car.keeping -= amount;
    return ns;
  }

  vector<State> transmit(int idx) const & {
    vector<State> nss;
    int inp = cbs[idx].inp;
    int out = cbs[idx].out;
    for (int amount = 1; amount <= cars[inp].keeping; ++amount) {
      State ns(*this);
      ns.cars[inp].keeping -= amount;
      int amountWithLoss = withLoss(amount, cbs[idx].loss);
      ns.cars[out].keeping += amountWithLoss;
      nss.push_back(ns);
    }

    return nss;
  }

  vector<State> generateNextStates() const & {
    vector<State> nss;
    for (int i = 0; i < cars.size(); ++i) {
      if (cars[i].ct == CTConsumer)
        nss.push_back(demand(i));
      if (cars[i].ct == CTGenerator)
        nss.push_back(fulfill(i));
    }

    for (int i = 0; i < cbs.size(); ++i) {
      auto transmitNss = transmit(i);
      for (auto const &s : transmitNss)
        nss.push_back(s);
    }

    return nss;
  }

  string toString() const & {
    string str = "";
    for (auto const &car : cars)
      str += car.toString() + " ";
    return str;
  }
};

int sumKeeping(vector<Carrier> const &cars) {
  int sum = 0;
  for (auto const &car : cars)
    sum += car.keeping;
  return sum;
}

int main() {
  State s;
  s.cars.emplace_back(s.cars.size(), CTConsumer, 200);
  s.cars.emplace_back(s.cars.size(), CTGenerator, 300);
  s.cbs.emplace_back(s.cbs.size(), 0, 1, 1000, 5);
  s.cbs.emplace_back(s.cbs.size(), 1, 0, 1000, 5);

  cout << "id:keeping:used\n";

  stack<State> q;
  set<string> inQueue;

  q.push(s);
  inQueue.insert(s.toString());
  while (!q.empty()) {
    State s = q.top();
    q.pop();

    for (int i = 0; i < s.depth; ++i)
      cout << " ";
    cout << s.toString() << "\n";

    for (auto &ns : s.generateNextStates()) {
      if (sumKeeping(ns.cars) > 1000)
        continue;
      string str = ns.toString();
      if (inQueue.count(str))
        continue;
      ns.depth = s.depth + 1;
      inQueue.insert(str);
      q.push(ns);
    }
  }
}
