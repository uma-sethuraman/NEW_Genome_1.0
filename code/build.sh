#!/bin/bash

# compile
cmake CMakeLists.txt && make

# clean up compilation files
find . -name "cmake_install.cmake" -type f -delete
find . -name "CMakeCache.txt" -type f -delete
find . -name "Makefile" -type f -delete
find . -name "libgenome.a" -type f -delete
find . -type d -name "CMakeFiles" -exec rm -rf {} \;
