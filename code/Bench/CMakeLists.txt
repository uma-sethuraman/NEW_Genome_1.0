cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the project name
project( bench )

# include source
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Genomes)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Genomes/GenomeLite)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Tests)


# include flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall" )

# Can manually add the sources using the set command as follows:
file( GLOB_RECURSE GENOME_SOURCES ../Genomes/*.cpp )
set( SIMPLE SimpleBench.cpp )
# set( CLONE CloneBench.cpp )

# add main executable
add_executable( simpleBench ${SIMPLE} ${GENOME_SOURCES} )
# add_executable( cloneBench ${CLONE} ${GENOME_SOURCES} )
