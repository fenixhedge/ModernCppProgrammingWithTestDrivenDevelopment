project(SnippetPublisher)
cmake_minimum_required(VERSION 2.6)

include_directories($ENV{RLOG_HOME} $ENV{BOOST_ROOT} $ENV{GTEST_HOME}/googletest/include $ENV{GTEST_HOME}/googlemock/include)
link_directories($ENV{RLOG_HOME}/rlog/.libs $ENV{GTEST_HOME}/build/lib)
set(Boost_USE_STATIC_LIBS ON)

add_subdirectory(rlog)
add_definitions(-std=c++11)

set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -DRLOG_COMPONENT=debug -Wall")
set(sources WavReader.cpp WavDescriptor.cpp)
set(testSources WavReaderTest.cpp)
add_executable(utest testmain.cpp ${testSources} ${sources})
add_executable(SnippetPublisher main.cpp ${sources})

find_package(Boost $ENV{BOOST_VERSION} COMPONENTS filesystem system)
#target_link_libraries(utest rlog)
target_link_libraries(utest rlogStub)
target_link_libraries(utest ${Boost_LIBRARIES})
target_link_libraries(utest gtest)
target_link_libraries(utest gmock)
target_link_libraries(utest pthread)

target_link_libraries(SnippetPublisher rlog)
target_link_libraries(SnippetPublisher ${Boost_LIBRARIES})
target_link_libraries(SnippetPublisher pthread)

