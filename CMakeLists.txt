cmake_minimum_required(VERSION 3.20)
project(sgrid)

set(CMAKE_CXX_STANDARD 20)

# find fmt, gtest packages and add them to the project
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_subdirectory(lib)
target_link_libraries(libsgrid fmt)


# enable_testing()
# add_executable(tests tests/correctness.cpp)
# target_link_libraries(tests GTest::gtest_main)
# target_link_libraries(tests libsgrid)
# target_link_libraries(tests fmt)
# include(GoogleTest)
# gtest_discover_tests(tests)

add_executable(main app/main.cpp)
target_link_libraries(main libsgrid)