project(design)
cmake_minimum_required(VERSION 2.6)

include_directories($ENV{GTEST_HOME}/googletest/include $ENV{GTEST_HOME}/googlemock/include)
link_directories($ENV{GTEST_HOME}/build/lib)
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(sources
	main.cpp
	PortfolioTest.cpp Portfolio.cpp)
add_executable(test ${sources})
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test pthread)
