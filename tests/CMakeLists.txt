cmake_minimum_required (VERSION 2.8)
project (assembler_interpreter)

include_directories(../ ../vendor/googletest/googletest/include ../vendor/googletest/googlemock/include)
add_executable(assembler_interpreter_tests assembler_interpreter_tests.cpp ../lib/assembler_interpreter.cpp)
target_link_libraries(assembler_interpreter_tests gmock gtest)
