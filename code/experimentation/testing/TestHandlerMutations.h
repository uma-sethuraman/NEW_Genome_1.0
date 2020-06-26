/**
 * \file TestHandlerMutations.h
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
namespace TestHandlerMutations
{
    void TestDeletion()
    {
        // initializing genomes 
        size_t size = 100;
        AbstractGenome* genome = new Genome(size);
        for (size_t i(0); i < size; i++) { *(genome->data()+i) = (Byte)i; }

        AbstractGenome* genomeLite = new GenomeLite(genome);
        auto handler = genomeLite->NewHandler();

        // // Delete Back
        // handler->DeleteMutation(99);
        // handler->MoveTo(98);
        // assert((Byte)98 == handler->Data());
        // assert(99 == genomeLite->size());

        // // Delete Middle
        // handler->DeleteMutation(50);
        // handler->MoveTo(50);
        // assert((Byte)51 == handler->Data());
        // assert(98 == genomeLite->size());

        // // Delete Middle
        // handler->DeleteMutation(10);
        // handler->MoveTo(10);
        // assert((Byte)11 == handler->Data());
        // assert(97 == genomeLite->size());

        // iterator over genome
        handler->DeleteMutation(70);
        handler->MoveTo(70);
        // assert((Byte)73 == handler->Data());
        // assert(96 == genomeLite->size());

        // Delete Front
        handler->DeleteMutation(0);
        handler->Reset();
        
        while(handler->HasNext())
        {
            std::cout << (int)handler->Data() << std::endl;
            handler->Next();
        }

        std::cout << (int)genomeLite->size() << std::endl;
        assert((Byte)1 == handler->Data());
        // assert(95 == genomeLite->size());


        delete handler;
        delete genomeLite;
        delete genome;

        std::cout << "Test Delete Mutation:" << BOLDGREEN << "Passed" << RESET << std::endl;
    }
}