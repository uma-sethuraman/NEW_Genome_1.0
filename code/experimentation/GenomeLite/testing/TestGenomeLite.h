/**
 * \file TestGenomeLite.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing genomeLite functions
 **/
#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include <algorithm>

#include "../../AbstractGenome.h"
#include "../../AbstractHandler.h"
#include "../Genome.h"
#include "../GenomeLite.h"

typedef char Byte; ///< Byte for easy viewing

#define RESET   "\033[0m"
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */

/** handler testing functions **/
namespace TestGenomeLite
{
    void TestCopyConstructor()
    {
        // initializing genomes 
        size_t size = 100;
        AbstractGenome* genome = new Genome(size);
        for (size_t i(0); i < size; i++) { *(genome->data()+i) = (Byte)i; }

        AbstractGenome* genomeLite = new GenomeLite(genome);
        AbstractGenome* copyGenome = genomeLite->Clone();

        auto handler = genomeLite->NewHandler();
        auto copyHandler = copyGenome->NewHandler();

        copyGenome->DeleteMutation(0);
        
        handler->MoveTo(0);
        assert(handler->Data() == (Byte)0);
        assert(genomeLite->size() == 100);

        copyHandler->MoveTo(0);
        assert(copyHandler->Data() == (Byte)1);
        assert(copyGenome->size() == 99);

        delete handler;
        delete copyHandler;
        delete genomeLite;
        delete copyGenome;
        delete genome;

        std::cout << "Test Copy Constructor:" << BOLDGREEN << "Passed" << RESET << std::endl;
    }

    void TestReallocation()
    {
        // initializing genomes 
        size_t size = 15;
        AbstractGenome* genome = new Genome(size);
        for (size_t i(0); i < size; i++) { *(genome->data()+i) = (Byte)i; }

        AbstractGenome* genomeLite = new GenomeLite(genome);

        Byte mutation[1] = {0x1};

        for (size_t i(0); i < 5; i++)
        {
            // genomeLite->Print();

            genomeLite->InsertMutation(rand()%size, mutation, sizeof(mutation));

            // std::cout << BOLDBLUE << "insertion " << i << RESET << std::endl;
            // genomeLite->Print();
            
        }


        delete genome;
        delete genomeLite;
    }


}