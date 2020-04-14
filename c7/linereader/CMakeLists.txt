cmake_minimum_required(VERSION 2.6)

include_directories($ENV{GTEST_HOME}/googletest/include $ENV{GTEST_HOME}/googlemock/include)
link_directories($ENV{GTEST_HOME}/build/lib)
add_definitions(-std=c++11)

set(sources
	main.cpp
	LineReaderTest.cpp LineReader.h)
add_executable(test ${sources})
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test pthread)
target_link_libraries(test rt)
