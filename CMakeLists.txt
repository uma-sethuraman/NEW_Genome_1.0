cmake_minimum_required(VERSION 3.16.0)

# C++17
set(CMAKE_CXX_STANDARD 17) 

project(TetianaGenome)

# Header files
set(HEADER_FILES code/Genomes/AbstractGenome.h code/Genomes/TestGenome.h code/Genomes/TetianaGenome.h code/Tools/utilities.h code/Tools/catch.hpp code/Tests/UmaChangelogTests.h)


# Source files
set(SOURCES code/main.cpp code/Genomes/AbstractGenome.cpp code/Genomes/TestGenome.cpp code/Genomes/TetianaGenome.cpp code/Tests/TetianaTests.cpp)


add_executable(TetianaGenome ${SOURCES} ${HEADER_FILES})

target_include_directories(TetianaGenome PUBLIC ${CMAKE_CURRENT_LIST_DIR}/code)
