cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the project name
project( test )
add_subdirectory( Genomes )

# include source
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Genomes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Genomes/GenomeLite)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Tests)



# include flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall" )

# Can manually add the sources using the set command as follows:
set( MAINEXEC main.cpp )

# add main executable
add_executable( test ${MAINEXEC} )
target_link_libraries( test genomes )
