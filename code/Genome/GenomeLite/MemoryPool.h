/**
 * \file MemoryPool.h
 * \author Victoria Cao
 * 
 * \brief class for a memory pool object to localize
 * memory allocations
 **/
#pragma once
#include <cstdint>
#include <cstring>

#include "SegmentNode.h"

/** class for a memory pool object **/
template < class T >
class MemoryPool
{
private:
    /// pool of memory
    T* Pool; 
    const size_t Size;
    size_t Tail = 0;

public:
    /** (deleted) default constructor **/
    MemoryPool() = delete;

    /** Constructor 
     * \param size **/
    MemoryPool(size_t size) 
        : Pool(new T[size]), Size(size) {}

    /** Copy constructor 
     * \param pool to copy from **/
    MemoryPool(const MemoryPool &pool)
        : Pool(new T[pool.Size]), Size(pool.Size)
    {
        std::memcpy(Pool, pool, Size*sizeof(T));
    }

    /** Destructor **/
    ~MemoryPool() { delete[] Pool; }

    /** Gets the index for next node to allocate
     * \return tail **/
    const size_t GetTail() { return Tail; }

    /** Allocates new node **/
    T* Allocate() { return &Pool[Tail++]; }
};