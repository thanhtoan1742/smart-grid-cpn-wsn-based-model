cmake_minimum_required(VERSION 3.20)
project(sgrid)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  plog
  GIT_REPOSITORY https://github.com/SergiusTheBest/plog.git
  GIT_TAG 1.1.9
)
FetchContent_MakeAvailable(plog)

add_subdirectory(lib)
target_link_libraries(libsgrid fmt)
target_link_libraries(libsgrid plog)


enable_testing()
add_executable(tests tests/correctness.cpp)
target_link_libraries(tests GTest::gtest_main)
target_link_libraries(tests libsgrid)
target_link_libraries(tests fmt)
include(GoogleTest)
gtest_discover_tests(tests)

add_executable(main app/main.cpp)
target_link_libraries(main libsgrid)