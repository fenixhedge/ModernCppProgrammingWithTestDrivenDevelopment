project(chapter7)
cmake_minimum_required(VERSION 2.6)

include_directories(include)
add_definitions(-std=c++11)

set(sources
		Book.cpp)
add_library(library ${sources})
target_link_libraries(library pthread)
target_link_libraries(library rt)
