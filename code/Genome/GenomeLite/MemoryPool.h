/**
 * \file MemoryPool.h
 * \author Victoria Cao
 * 
 * \brief class for a memory pool object to localize
 * memory allocations
 **/
#pragma once

#include <memory>
#include <cstdint>
#include <cstring>
#include <vector>

#include "SegmentNode.h"

typedef std::byte Byte;

/** class for a memory pool object **/
class MemoryPool
{
private:
    /// pool of memory
    SegmentNode* Pool; 
    const size_t Size;
    size_t Tail = 1;

public:
    /** (deleted) default constructor **/
    MemoryPool() = delete;

    /** Constructor 
     * \param size **/
    MemoryPool(size_t size) 
        : Pool(new SegmentNode[size]), Size(size) {}

    /** Copy constructor 
     * \param pool to copy from **/
    MemoryPool(const MemoryPool &pool)
        : Pool(new SegmentNode[pool.Size]), Size(pool.Size)
    {
        std::memcpy(Pool, pool.Pool, Size*sizeof(SegmentNode));
    }

    /** Destructor **/
    ~MemoryPool() { delete[] Pool; }

    /** Gets the index for next node to allocate
     * \return tail **/
    const size_t GetTail() { return Tail; }

    /** Gets node at index
     * \param index
     * \returns node at index **/
    SegmentNode* At(size_t index) { return &Pool[index]; }

    /** Allocates new node **/
    SegmentNode* Allocate(std::shared_ptr< GeneSegment > gene, Byte* start, size_t size) 
    { 
        Pool[Tail] = SegmentNode(gene, Tail, start, size);
        return &Pool[Tail++]; 
    }

    /** Allocates new node **/
    SegmentNode* Allocate(size_t maxSize, size_t size) 
    { 
        Pool[Tail] = SegmentNode(maxSize, size, Tail);
        return &Pool[Tail++]; 
    }

        /** Allocates new node **/
    SegmentNode* Allocate(size_t size) 
    { 
        Pool[Tail] = SegmentNode(size, Tail);
        return &Pool[Tail++]; 
    }


};