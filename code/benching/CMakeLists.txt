cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# expected behaviour
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# set the project name
project(bench)

# get all *.cpp files recursively
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/../experimentation/*.cpp BenchMutation.cpp)
add_executable(bench ${SRC_FILES})
