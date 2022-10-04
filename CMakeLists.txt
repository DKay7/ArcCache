cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(arc_comparational_test VERSION 2.0)

add_subdirectory(ARC)
add_subdirectory(PerfectCache)
add_subdirectory(UnitTests)