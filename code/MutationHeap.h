/**
 * \file MutationHeap.h
 * \author Victoria Cao
 * 
 * \brief Managing pointers to mutation
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>
#include <vector>

#include "Mutation.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class MutationHeap
{
private:
    std::vector< std::pair< std::shared_ptr< Mutation >, int > > Heap; ///< managing Mutation blocks of memory

public:
    /** Default constructor **/
    MutationHeap() {}

    /** Creates a mutation and adds it to the heap
     * \param value New value for the mutation
     * \return pointer to the mutation object **/
    template < typename T >
    std::shared_ptr<Mutation> AddMutation(T value)
    {
        std::shared_ptr< Mutation > newMutation = std::make_shared< Mutation >(value);
        std::pair< std::shared_ptr< Mutation >, int > mutationPair = { newMutation, 1 };
        Heap.push_back( mutationPair );

        return newMutation;
    }

    /** Deconstructor **/
    ~MutationHeap() { Heap.clear(); }

};