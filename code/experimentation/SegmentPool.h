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

typedef char Byte; // c++17 std::byte doesn't always work

/** Implementation of a genom object **/
class SegmentPool
{
private:
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

    /** Deconstructor **/
    ~SegmentPool() { delete[] Pool; }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t GetSize() { return Size; }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const bool IsFull() { return Tail < Size; }

    /** Creates a new node in the pool
     * \param data to make SegmentNOde from
     * \return pointer to newly allocated node **/
    SegmentNode* CreateNode(std::shared_ptr< GeneSegment> data)
    {
        if (Tail < Size)
        {
            SegmentNode* newNode = &Pool[Tail]; // give out next open space for node
            *newNode = SegmentNode(data); // assign node the data

            Tail++; // move tail

            return newNode;
        }

        return nullptr;
    }

    /** Creates a new node in the pool
     * \param data to make SegmentNOde from
     * \param start Start of data within the segment
     * \param size of data
     * \return pointer to newly allocated node **/
    SegmentNode* CreateNode(std::shared_ptr< GeneSegment> data, Byte* start, size_t size) 
    {
        if (Tail < Size)
        {
            SegmentNode* newNode = &Pool[Tail]; // give out next open space for node
            *newNode = SegmentNode(data, start, size); // assign node the data

            Tail++; // move tail

            return newNode;
        }

        return nullptr;
    }

};