cmake_minimum_required(VERSION 2.6)

include_directories($ENV{GTEST_HOME}/googletest/include $ENV{GTEST_HOME}/googlemock/include ../library/include)
link_directories($ENV{GTEST_HOME}/build/lib)
add_definitions(-std=c++11)

set(sources
		main.cpp)
add_library(libraryTest ${sources})
target_link_libraries(libraryTest gtest)
target_link_libraries(libraryTest gmock)
target_link_libraries(libraryTest library)
target_link_libraries(libraryTest pthread)
target_link_libraries(libraryTest rt)
