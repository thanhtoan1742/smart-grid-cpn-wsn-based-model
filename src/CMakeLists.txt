cmake_minimum_required(VERSION 3.20)
project(wsn-based-smart-grid)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp Carrier.cpp CircuitBreaker.cpp State.cpp)
