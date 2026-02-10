cmake_minimum_required(VERSION 4.1)
project(GW)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TESTS "tests/*.cpp")
add_executable(GW main.cpp ${SOURCES} ${TESTS})

target_include_directories(GW PRIVATE src)
target_include_directories(GW PRIVATE tests)
