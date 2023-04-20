cmake_minimum_required(VERSION 3.20)
project(sgrid)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_subdirectory(lib)


enable_testing()
add_executable(runtest tests/main.cpp)
target_link_libraries(runtest GTest::gtest_main)
target_link_libraries(runtest libsgrid)

add_executable(main app/main.cpp)
target_link_libraries(main libsgrid)