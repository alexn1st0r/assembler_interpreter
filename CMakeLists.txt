cmake_minimum_required (VERSION 2.8)
project (assembler_interpreter)

set(CMAKE_CXX_FLAGS  "-std=c++11 -g -ggdb -O0")

add_subdirectory(vendor/googletest)
add_subdirectory(tests)
