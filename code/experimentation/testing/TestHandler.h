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
#include "../Genome.h"
#include "../GenomeLite.h"
#include "../GeneSegment.h"
#include "../handlers/MutatorHandler.h"

typedef char Byte; ///< Byte for easy viewing

/** handler testing functions **/
namespace TestHandler
{
    const SegmentNode* findSegment(MutatorHandler handler, size_t index)
    {
        handler.reset();
        for (size_t i(0); i < index; i++)
        {
            handler.next();
        }

        return handler.segment();
    }

    void testSearch()
    {
        std::cout << "\nTest Search" << std::endl;
        /// creating genome
        AbstractGenome* genome = new Genome(100);

        ///init genome
        for (size_t i = 0; i < genome->size(); i++)
        {
            genome->data()[i] = i;
        }

        GenomeLite* genomeMutation = new GenomeLite(genome);
        MutatorHandler handler(genomeMutation);

        /// Set mutations
        handler.reset();
        std::vector<size_t> inserts = {26};
        while (handler.index() < genomeMutation->size())
        {
            auto val = *handler;
            if (std::find(inserts.begin(), inserts.end(), val) != inserts.end() )
            {
                auto mutation = std::make_shared< GeneSegment >((Byte)1);
                handler.PointMutation(mutation);
            }	
            handler.next();
        }

        // genomeMutation->print();

        handler.reset();
        handler.moveTo(27);

        assert(handler.segment() == findSegment(handler, 27));

        std::cout << "Search: passed" << std::endl;

        delete genome;
        delete genomeMutation;

    }
}