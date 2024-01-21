cmake_minimum_required(VERSION 3.20)
project(Google_tests)

include(CTest)
enable_testing()

add_subdirectory(lib)
#add_subdirectory(source)
include_directories(${gtest_SOURCE_DIR} /include ${gtest_SOURCE_DIR})

set(test_SRCS source/dijkstra_tests.cpp source/graph_tests.cpp)
add_executable(unit_tests source/tests.cpp ${test_SRCS})
target_link_libraries(unit_tests gtest gmock core)
add_test(unit_tests unit_tests)

#target_link_libraries(unit_tests gtest gtest_main)
target_include_directories(unit_tests PUBLIC ../source/)
target_include_directories(unit_tests PUBLIC ../source/graph)

#===============

#project(google_tests)
#add_subdirectory(lib)