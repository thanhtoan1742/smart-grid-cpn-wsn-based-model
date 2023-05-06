cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

add_library(libsgrid SHARED)
target_include_directories(libsgrid PUBLIC .)
target_sources(
    libsgrid PUBLIC
    sgrid/Percentage.cpp
    sgrid/Power.cpp
    sgrid/PowerSystem.cpp
    sgrid/TransmissionLine.cpp
    sgrid/Grid.cpp
    sgrid/PowerSystemState.cpp
    sgrid/State.cpp
    sgrid/Outcome.cpp
    sgrid/StateExplorer.cpp
    sgrid/utils.cpp
)