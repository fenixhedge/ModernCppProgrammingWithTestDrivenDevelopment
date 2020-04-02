project(design)
cmake_minimum_required(VERSION 2.6)

include_directories($ENV{GTEST_HOME}/googletest/include $ENV{GTEST_HOME}/googlemock/include $ENV{JSONCPP_HOME}/include)
link_directories($ENV{GTEST_HOME}/build/lib $ENV{JSONCPP_HOME}/build/src/lib_json)
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(sources
	main.cpp
	CurlHttp.cpp CurlHttpTest.cpp
	AddressExtractorTest.cpp AddressExtractor.cpp)
add_executable(test ${sources})
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test curl)
target_link_libraries(test jsoncpp)
target_link_libraries(test pthread)
