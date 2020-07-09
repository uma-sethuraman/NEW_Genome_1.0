/**
 * \file SegmentPool.h
 * \author Victoria Cao
 * 
 * \brief SegmentPool to localize all nodes for a SegmentTree
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include"SegmentNode.h"

typedef std::byte Byte; // c++17 std::byte doesn't always work

/** Implementation of a genom object **/
class SegmentPool
{
private:
    friend class SegmentTree; 

    SegmentNode* Pool; ///< new byte of data
    const size_t Size;
    
    size_t Tail = 0;

public:
    /** (deleted) Default Constructor **/
    SegmentPool() = delete;

    /** Constructor 
     * \param size value to put into gene **/
    SegmentPool(size_t size) 
        : Pool(new SegmentNode[size]), Size(size) {}

    /** Copy constructor
     * \param pool **/
    SegmentPool(const SegmentPool &pool)
        : Pool(new SegmentNode[pool.Size]), Size(pool.Size), Tail(pool.Tail)
    {
        std::memcpy(Pool, pool.Pool, Size*sizeof(SegmentNode));
    }

    /** Deconstructor **/
    ~SegmentPool() { delete[] Pool; }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t GetSize() { return Size; }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t GetTail() { return Tail; }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const bool IsFull() { return Tail < Size; }

    /** Gets node at index from array
     * \return node at index **/
    SegmentNode* At(size_t index) 
    { 
        return &Pool[index]; 
    }

    SegmentNode* Allocate() 
    {
        Pool[Tail] = SegmentNode(); // assign node the data
        return &Pool[Tail++];

        return nullptr;
    }

    void Print()
    {
        for (size_t i(0); i < Tail; i++)
        {
            Pool[i].Print();
        }
    }

};