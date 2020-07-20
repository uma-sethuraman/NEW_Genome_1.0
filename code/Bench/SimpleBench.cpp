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
#include "TestGenome.h"

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
    size_t numMutations = 10000; // 10,000

    std::string name = "GenomeLite";


    // Nested Insertion
    std::cout << "BENCHING: Insertion" << std::endl;
    printTime();
    std::cout << "\n" << name << ": \nSize\t" << size << "\nMutations\t" << numMutations << std::endl;  

    BENCHMARK("Normal Insertion") 
    {
        AbstractGenome* genome = new GenomeLite(size);

        for (size_t i(0); i < numMutations; ++i)
        {
            genome->insert(i+2, std::vector< Byte >(1, (Byte)i));
        }

        delete genome;
    };

    BENCHMARK("Nested Insertion") 
    {
        AbstractGenome* genome = new GenomeLite(size);

        for (size_t i(0); i < numMutations; ++i)
        {
            genome->insert(i, std::vector< Byte >(2, (Byte)i));
        }

        delete genome;
    };

}



TEST_CASE("Deletion Benchmarks", "[benchmark]") 
{
    freopen("bench.log","a",stdout);

    // initializing genomes 
    size_t size = 500000;
    size_t numMutations = 10000; // 10,000

    std::string name = "GenomeLite";

    // Nested Insertion
    std::cout << "BENCHING: Deletion" << std::endl;
    printTime();
    std::cout << "\n" << name << ": \nSize\t" << size << "\nMutations\t" << numMutations << std::endl;  

    BENCHMARK("Front Deletion") 
    {
        AbstractGenome* genome = new GenomeLite(size);

        for (size_t i(0); i < numMutations; ++i)
        {
            genome->remove(i, 1);
        }

        delete genome;
    };

    BENCHMARK("Back Deletion") 
    {
        AbstractGenome* genome = new GenomeLite(size);

        for (size_t i(0); i < numMutations; ++i)
        {
            genome->remove(size-i-1, 1);
        }

        delete genome;
    };


}


TEST_CASE("Overwrite Benchmarks", "[benchmark]") 
{
    freopen("bench.log","a",stdout);

    // initializing genomes 
    size_t size = 500000;
    size_t numMutations = 10000; // 10,000

    std::string name = "GenomeLite";

    // Nested Insertion
    std::cout << "BENCHING: Overwrites" << std::endl;
    printTime();
    std::cout << "\n" << name << ": \nSize\t" << size << "\nMutations\t" << numMutations << std::endl;  

    BENCHMARK("Normal Overwrites") 
    {
        AbstractGenome* genome = new GenomeLite(size);

        for (size_t i(0); i < numMutations; ++i)
        {
            genome->overwrite(i, std::vector<Byte>(1, (Byte)i));
        }

        delete genome;
    };

    BENCHMARK("Whole Genome Overwrite") 
    {
        AbstractGenome* genome = new GenomeLite(size);

        genome->overwrite(0, std::vector<Byte>(size, (Byte)0));

        delete genome;
    };

}
