cmake_minimum_required(VERSION 3.24)
project(Dijkstra)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(google_tests)

set(SOURCE_FILES source/main.cpp source/dijkstra.cpp)
add_library(core ${SOURCE_FILES})
add_executable(Dijkstra source/main.cpp)
target_link_libraries(Dijkstra core)

