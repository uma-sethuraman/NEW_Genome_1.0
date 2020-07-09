cmake_minimum_required(VERSION 3.16.0)

# C++17
set(CMAKE_CXX_STANDARD 17) 

project(TetianaGenome)

# Header files
set(HEADER_FILES code/AbstractGenome.h code/TestGenome.h code/TetianaGenome.h code/TetianaTests.h code/utilities.h code/catch.hpp)

# Source files
set(SOURCES code/main.cpp code/AbstractGenome.cpp code/TestGenome.cpp code/TetianaGenome.cpp)

add_executable(TetianaGenome ${SOURCES} ${HEADER_FILES})
