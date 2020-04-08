cmake_minimum_required(VERSION 2.6)

include_directories($ENV{GTEST_HOME}/googlemock/include $ENV{GTEST_HOME}/googletest/include ${CMAKE_SOURCE_DIR}/library/include)
link_directories($ENV{GTEST_HOME}/build/lib ${CMAKE_SOURCE_DIR}/library)
add_definitions(-std=c++11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(sources 
   main.cpp 
   BarcodeUtilTest.cpp
   BookTest.cpp
   BranchAccessTest.cpp
   BranchServiceTest.cpp
   BranchTest.cpp
   CatalogTest.cpp
   ClassificationServiceTest.cpp
   HoldingBarcodeTest.cpp
   HoldingServiceTest.cpp
   HoldingTest.cpp
   InventoryReportTest.cpp
   KeyedFilePersistenceTest.cpp
   KeyedMemoryPersistenceTest.cpp
   PatronAccessTest.cpp
   PatronServiceTest.cpp
   PatronTest.cpp
   PersistenceTest.cpp
   ReportMailerTest.cpp
   ScannerStateCheckinTest.cpp
   ScannerStateCheckoutTest.cpp
   ScannerStateInventoryTest.cpp
   ScannerStateWaitingTest.cpp
   ScannerTestData.cpp
   StreamLineReaderTest.cpp
   TimestampSourceTest.cpp
   VectorReaderTest.cpp
   )
add_executable(test ${sources})
target_link_libraries(test library)
target_link_libraries(test gmock)
target_link_libraries(test gtest)
target_link_libraries(test pthread)

