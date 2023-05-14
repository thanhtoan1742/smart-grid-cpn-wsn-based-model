from random import randint, choices, random, expovariate
import math
from dataclasses import dataclass

N = 50
N_GEN = 7
N_CON = 15
N_BUS = N - N_GEN - N_CON
N_TL_EXTRA = 20

def print_ps(id, typ):
  if typ == "CON":
    t = "PowerSystemType::Consumer"
  elif typ == "GEN":
    t = "PowerSystemType::Generator"
  else:
    t = "PowerSystemType::Bus"

  if typ == "GEN":
    cap = randint(10, 100) * 100
    print(f".createPowerSystem({id}, {t}, {cap})")
  elif typ == "CON":
    cap = randint(10, 100) * 10
    print(f".createPowerSystem({id}, {t}, {cap})")
  else:
    print(f".createPowerSystem({id}, {t})")

def print_tl(i, o):
  cap = randint(5, 20) * 10000
  loss = round(expovariate(0.4)/100, 2)
  # print(f".createTransmissionLine({i}, {o}, {cap}, {loss})")
  # print(f".createTransmissionLine({i}, {o}, Power::maxPower, {loss})")
  print(f".createTransmissionLine({i}, {o}, {cap}, {0.01})")

gen_ids = list(range(N_GEN))
con_ids = list(range(N_GEN, N_GEN + N_CON))
bus_ids = list(range(N_GEN + N_CON, N))

graph = []
for x in bus_ids:
  y = choices(list(set(gen_ids).difference(set([x]))))[0]
  graph.append((x, y))

for x in gen_ids + con_ids:
  y = choices(bus_ids)[0]
  graph.append((x, y))

all_ids = gen_ids + con_ids + bus_ids
for _ in range(N_TL_EXTRA):
  x = choices(all_ids)[0]
  y = choices(list(set(all_ids).difference(set([x]))))[0]
  graph.append((x, y))


print("Grid grid = GridFactory()")
for id in gen_ids:
  print_ps(id, "GEN")
for id in con_ids:
  print_ps(id, "CON")
for id in bus_ids:
  print_ps(id, "BUS")
for x, y in graph:
  print_tl(x, y)
print(".createGrid();")
