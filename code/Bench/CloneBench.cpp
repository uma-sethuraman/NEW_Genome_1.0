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
    std::string name = "UmaGenome";
    std::string file = "logs/"+name+".log";
    freopen(file.c_str(), "a", stdout);

    // initializing mutation list 
    std::vector<size_t> mutations = randomList(size);


    // Nested Insertion
    std::cout << "BENCHING: Insertion" << std::endl;
    printTime();
    std::cout << "\n" << name << ": \nSize\t" << size << "\nMutations\t" << mutations.size()  << std::endl;  

    BENCHMARK("Random Insertion") 
    {
        AbstractGenome* genome = new UmaGenome(size);

        for (const auto& site : mutations)
        {
            genome->insert(site, std::vector< Byte >(1, (Byte)1));
        }

        delete genome;
    };

}
