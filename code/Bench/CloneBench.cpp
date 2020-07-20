/**
 * \file Bench.cpp
 * \author Victoria Cao
 * 
 * \brief Tests for benchmarking genomes.
 * Terminal output is saved to bench.log
 * 
 **/

#include <ctime>
#include <string>

#include "AbstractGenome.h"
#include "GenomeLite.h"

#define CATCH_CONFIG_MAIN 
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch.hpp"

typedef std::byte Byte;

void printTime() {
    time_t t = time(0); 
    struct tm * timeStruct = localtime(&t);
    std::cout << (timeStruct->tm_year) << '-' << (timeStruct->tm_mon) << '-'<<  (timeStruct->tm_mday);
    std::cout << "\t" << (timeStruct->tm_hour) << ":" << (timeStruct->tm_min) << ":" << (timeStruct->tm_sec) <<  std::endl;
}


TEST_CASE("Insertion Benchmarks", "[benchmark]") 
{
    freopen("bench.log","a",stdout);

    // initializing genomes 
    size_t size = 500000;
    std::vector< AbstractGenome* > Parents;
    std::string name = "GenomeLite";

    for(size_t i (0); i < 5; ++i)
    {
        AbstractGenome* genome = new GenomeLite(size);
        Parents.push_back();
    }

    size_t numMutations = 100000; // 10,000

    // clone variables
    std::vector< AbstractGenome* > Children;


    // Nested Insertion
    std::cout << "BENCHING: Clone Mutation" << std::endl;
    printTime();
    std::cout << "\n" name ": \nSize\t" << size << "\nMutations\t" << numMutations << std::endl;  

    BENCHMARK("Clone Creation") 
    {
        for(size_t i (0); i < 95; ++i)
        {
            Children.push_back(Parents[i%Parents.size()]->clone());
        }
    };


    // delete pointers
    for(auto &genome : Parents)
    {
        delete genome;
    }

    for(auto &genome : Children)
    {
        delete genome;
    }
}
