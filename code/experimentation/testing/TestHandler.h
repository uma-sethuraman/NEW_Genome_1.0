/**
 * \file TestHandler.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing handler functions
 **/
#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>


#include "../AbstractGenome.h"
#include "../AbstractHandler.h"

#include "../Genome.h"
#include "../GenomeLite.h"


typedef char Byte; ///< Byte for easy viewing

#define RESET   "\033[0m"
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */

/** handler testing functions **/
namespace TestHandler
{
    void TestConstructor()
    {
        // initializing genomes 
        size_t size = 100;
        AbstractGenome* genome = new Genome(size);
        for (size_t i(0); i < size; i++) { *genome->data() = (Byte)i; }

        AbstractGenome* genomeLite = new GenomeLite(genome);

        auto handler = genomeLite->CreateHandler();

        delete genomeLite;
        delete genome;

        std::cout << "Test Constructor:" << BOLDGREEN << "Passed" << RESET << std::endl;
    }
}