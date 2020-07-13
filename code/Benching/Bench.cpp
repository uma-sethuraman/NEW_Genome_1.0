/**
 * \file Bench.cpp
 * \author Victoria Cao
 * 
 * \brief Tests for benchmarking genomes.
 * Terminal output is saved to bench.log
 * 
 **/

#include <ctime>

#include "../AbstractGenome.h"
#include "../TestGenome.h"
#include "../GenomeLite/GenomeLite.h"


#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch.hpp"

void printTime() {
    time_t t = time(0); 
    struct tm * timeStruct = localtime(&t);
    std::cout << (timeStruct->tm_year) << '-' << (timeStruct->tm_mon) << '-'<<  (timeStruct->tm_mday);
    std::cout << "\t" << (timeStruct->tm_hour) << ":" << (timeStruct->tm_min) << ":" << (timeStruct->tm_sec) <<  std::endl;
}


TEST_CASE("Size 500000", "[benchmark]") 
{
    freopen("bench.log","a",stdout);

    // initializing genomes 
    size_t size = 500000;
    AbstractGenome* genome = new TestGenome(size);
    std::string name = "TestGenome";

    size_t numMutations = 100000; // 10,000



    // Nested Insertion
    printTime();
    std::cout << "\n" << name << ": \n" << "Size\t" << size << "\nMutations\t" << numMutations << std::endl;  
    
    BENCHMARK("Nested Insertion") 
    {
        for (size_t i(0); i < numMutations; ++i)
        {
            genome->insert(i, std::vector< Byte >(2, (Byte)i));
        }
    };


    // Mass Deletion
    printTime();
    std::cout << "\n" << name << ": \n" << "Size\t" << size << "\nMutations\t" << numMutations << std::endl;  
    
    BENCHMARK("Mass Deletion") 
    {
        for (size_t i(0); i < numMutations; ++i)
        {
            genome->remove(i, 2);
        }
    };

    delete genome;
}