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
#include <fstream>
#include <vector>

#include "AbstractGenome.h"
#include "GenomeLite.h"
#include "TestGenome.h"
#include "UmaGenome.h"

#include "benchUtilities.h"

#define CATCH_CONFIG_MAIN 
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch.hpp"

typedef std::byte Byte;

size_t size = 500000;


TEST_CASE("Overwrite Benchmarks", "[benchmark]") 
{
    std::string name = "UmaGenome";
    std::string file = "logs/"+name+".log";
    // freopen(file.c_str(), "a", stdout);

    // initializing mutation list 
    std::vector<size_t> mutations = randomList(size);

    for (const auto& site : mutations)
    {
        std::cout << site << std::endl;
    }

    // Nested Insertion
    std::cout << "BENCHING: Overwrites" << std::endl;
    printTime();
    std::cout << "\n" << name << ": \nSize\t" << size << "\nMutations\t" << mutations.size() << std::endl;  

    BENCHMARK("Random Overwrites") 
    {
        AbstractGenome* genome = new UmaGenome(size);

        for (const auto& site : mutations)
        {
            genome->overwrite(site, std::vector< Byte >(1, (Byte)1));
        }

        delete genome;
    };

    BENCHMARK("Whole Genome Overwrite") 
    {
        AbstractGenome* genome = new UmaGenome(size);

        genome->overwrite(0, std::vector<Byte>(size, (Byte)0));

        delete genome;
    };

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


TEST_CASE("Deletion Benchmarks", "[benchmark]") 
{
    std::string name = "UmaGenome";
    std::string file = "logs/"+name+".log";
    freopen(file.c_str(), "a", stdout);

    // initializing mutation list 
    std::vector<size_t> mutations = randomList(size);


    // Nested Insertion
    std::cout << "BENCHING: Deletion" << std::endl;
    printTime();
    std::cout << "\n" << name << ": \nSize\t" << size << "\nMutations\t" << mutations.size() << std::endl;  

    BENCHMARK("Random Deletion") 
    {
        AbstractGenome* genome = new UmaGenome(size);

        for (size_t i(0); i < mutations.size(); i++)
        {
            genome->remove(mutations[i]%(size-i-1), 1);
        }

        delete genome;
    };

}


