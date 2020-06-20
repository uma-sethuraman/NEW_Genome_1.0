/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief Segment node class for segmentation of data
 * implementation
 **/

#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <iostream>

#include "GeneSegment.h"


typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genome object **/
class SegmentNode
{
private:
    /// Data variables
    std::shared_ptr< GeneSegment > Segment;
	Byte* Start; ///< Start of memory
    size_t Size; ///< Size of segment

    /// Linked list variables
    SegmentNode* Prev = nullptr;
    SegmentNode* Next = nullptr;

public:
    /** (deleted) Default Constructor */
    SegmentNode() = delete;

    /** Constructor
     * \param start beginning of segment
     * \param size size of segment */
    SegmentNode(std::shared_ptr< GeneSegment> segment) 
        : Segment(segment), Start(segment->data()), Size(segment->size())
    {
    }


    /** Constructor
     * \param start beginning of segment
     * \param size size of segment */
    SegmentNode(std::shared_ptr< GeneSegment> segment, Byte* start, size_t size) 
        : Segment(segment), Start(start), Size(size)
    {
    }

    /** Deconstructor **/
    ~SegmentNode() { delete Next; }

    /** Gets size
     * \returns size of segment **/
    const size_t size() { return Size; }

    /** Gets size
     * \returns size of segment **/
    SegmentNode* next() { return Next; }

    /** Gets size
     * \returns size of segment **/
    SegmentNode* prev() { return Prev; }

    /** Gets data from Segment
     * \returns size of segment **/
    const Byte getData(size_t index)
    {
        if (index < Size)
            return *Start+index;
        else
        {
            std::cout << "trying to get " << (int)index << "from" << std::endl;
            print();
            exit(1);
        }
        
    }

    /** Sets size
     * \returns size of segment **/
    void SetNext(SegmentNode* next) { Next = next; }

    /** Setes size
     * \returns size of segment **/
    void SetPrev(SegmentNode* prev) { Prev = prev; }


    void TruncateLeft(size_t cutSize = 1);
    void TruncateRight(size_t cutSize = 1);
    SegmentNode* Cut(size_t index);

    void print()
    {
        std::cout << "SegmentNode " << this << std::endl;
        std::cout << "prev " << std::hex << Prev << std::endl;
        std::cout << "B " << (unsigned long)Start << std::endl;
        std::cout << "E " << (unsigned long)Start+Size << std::endl;
        
        for (size_t i = 0; i < Size; i++)
        {
            std::cout << (int)(*(Start+i)) << ", ";
        }
        std::cout << std::endl;

        std::cout << "next " << Next << std::endl;
        std::cout << std::endl;
    }



};
