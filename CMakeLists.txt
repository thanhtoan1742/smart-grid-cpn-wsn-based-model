cmake_minimum_required(VERSION 3.20)
project(sgrid)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libsgrid)

add_executable(main app/main.cpp)
target_link_libraries(main libsgrid)