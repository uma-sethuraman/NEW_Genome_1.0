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

#include "../../AbstractGenome.h"
#include "../../AbstractHandler.h"
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

        // Delete Back
        genomeLite->DeleteMutation(99);
        handler->MoveTo(98);
        assert((Byte)98 == handler->Data());
        assert(99 == genomeLite->size());

        // Delete Middle
        genomeLite->DeleteMutation(50);
        handler->MoveTo(50);
        assert((Byte)51 == handler->Data());
        assert(98 == genomeLite->size());

        // // Delete Middle
        genomeLite->DeleteMutation(10);
        handler->MoveTo(10);
        assert((Byte)11 == handler->Data());
        assert(97 == genomeLite->size());

        // iterator over genome
        genomeLite->DeleteMutation(70);
        handler->MoveTo(70);
        assert((Byte)73 == handler->Data());
        assert(96 == genomeLite->size());

        // Delete Front
        genomeLite->DeleteMutation(0);
        handler->MoveTo(0);
        assert((Byte)1 == handler->Data());
        assert(95 == genomeLite->size());


        delete handler;
        delete genomeLite;
        delete genome;

        std::cout << "Test Delete Mutation:" << BOLDGREEN << "Passed" << RESET << std::endl;
    }

    void TestInsertion()
    {
        // initializing genomes 
        size_t size = 100;
        AbstractGenome* genome = new Genome(size);
        for (size_t i(0); i < size; i++) { *(genome->data()+i) = (Byte)i; }

        AbstractGenome* genomeLite = new GenomeLite(genome);
        auto handler = genomeLite->NewHandler();

        // Delete Back
        Byte mutation[1] = {0x1};

        genomeLite->InsertMutation(99, mutation, sizeof(mutation));
        handler->MoveTo(99);
        assert((Byte)1 == handler->Data());
        assert(101 == genomeLite->size());

        // Delete Middle
        genomeLite->InsertMutation(50, mutation, sizeof(mutation));
        handler->MoveTo(50);
        assert((Byte)1 == handler->Data());
        assert(102 == genomeLite->size());

        // // Delete Middle
        genomeLite->InsertMutation(10, mutation, sizeof(mutation));
        handler->MoveTo(10);
        assert((Byte)1 == handler->Data());
        assert(103 == genomeLite->size());

        // iterator over genome
        genomeLite->InsertMutation(70, mutation, sizeof(mutation));
        handler->MoveTo(70);
        assert((Byte)1 == handler->Data());
        assert(104 == genomeLite->size());

        // Delete Front
        genomeLite->InsertMutation(0, mutation, sizeof(mutation));
        handler->MoveTo(0);
        assert((Byte)1 == handler->Data());
        assert(105 == genomeLite->size());

        delete handler;
        delete genomeLite;
        delete genome;

        std::cout << "Test Insert Mutation:" << BOLDGREEN << "Passed" << RESET << std::endl;
    }

    void TestPoint()
    {
        // initializing genomes 
        size_t size = 100;
        AbstractGenome* genome = new Genome(size);
        for (size_t i(0); i < size; i++) { *(genome->data()+i) = (Byte)i; }

        AbstractGenome* genomeLite = new GenomeLite(genome);
        auto handler = genomeLite->NewHandler();

        // Delete Back
        Byte mutation[1] = {0x1};

        genomeLite->PointMutation(99, mutation, sizeof(mutation));
        handler->MoveTo(99);
        assert((Byte)1 == handler->Data());
        assert(100 == genomeLite->size());

        // Delete Middle
        genomeLite->PointMutation(50, mutation, sizeof(mutation));
        handler->MoveTo(50);
        assert((Byte)1 == handler->Data());
        assert(100 == genomeLite->size());

        // // Delete Middle
        genomeLite->PointMutation(10, mutation, sizeof(mutation));
        handler->MoveTo(10);
        assert((Byte)1 == handler->Data());
        assert(100 == genomeLite->size());

        // iterator over genome
        genomeLite->PointMutation(70, mutation, sizeof(mutation));
        handler->MoveTo(70);
        assert((Byte)1 == handler->Data());
        assert(100 == genomeLite->size());

        // Delete Front
        genomeLite->PointMutation(0, mutation, sizeof(mutation));
        handler->MoveTo(0);
        assert((Byte)1 == handler->Data());
        assert(100 == genomeLite->size());


        delete handler;
        delete genomeLite;
        delete genome;

        std::cout << "Test Insert Mutation:" << BOLDGREEN << "Passed" << RESET << std::endl;
    }
}