cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

add_library(libsgrid STATIC)
target_include_directories(libsgrid PUBLIC .)
target_sources(
    libsgrid PUBLIC
    libsgrid/Percentage.cpp
    libsgrid/Power.cpp
    libsgrid/PowerSystem.cpp
    libsgrid/TransmissionLine.cpp
    libsgrid/Grid.cpp
    libsgrid/PowerSystemState.cpp
    libsgrid/State.cpp
    libsgrid/StateExplorer.cpp
    libsgrid/utils.cpp
)