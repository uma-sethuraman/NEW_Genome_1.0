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
    friend class SegmentTree;

    /// Data variables
    std::shared_ptr< GeneSegment > Data;
	Byte* Start; ///< Start of memory
    size_t Size; ///< Size of data

    /// Linked list variables
    SegmentNode* Left = nullptr;
    SegmentNode* Right = nullptr;
    SegmentNode* Parent = nullptr;

    int Height = 0;
    size_t Weight = 0;


public:
    /** (deleted) Default Constructor */
    SegmentNode() = delete;

    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< GeneSegment> data) 
        : Data(data), Start(data->data()), Size(data->size()), Weight(data->size()) {}


    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< GeneSegment> data, Byte* start, size_t size) 
        : Data(data), Start(start), Size(size), Weight(size) {}

    /** Deconstructor **/
    ~SegmentNode() { }

    /** Gets size
     * \returns size of data **/
    const size_t size() { return Size; }

    /** Gets size
     * \returns size of data **/
    SegmentNode* next() { return Right; }

    /** Gets size
     * \returns size of data **/
    SegmentNode* prev() { return Left; }

    /** Gets data from Segment
     * \returns size of data **/
    const Byte getData(size_t index)
    {
        if (index < Size)
            return *Start+index;
        else
        {
            std::cout << "Errpr: trying to get data from index " << (int)index << std::endl;
            print();
            exit(1);
        }
        
    }


    void TruncateLeft(size_t cutSize = 1);
    void TruncateRight(size_t cutSize = 1);
    SegmentNode* Cut(size_t index);

    void print()
    {
        std::cout << "SegmentNode " << this << std::endl;
        std::cout << "weight " << Weight << " size " << Size << " height " << Height << std::endl;
        std::cout << "prev " << std::hex << Left << std::endl;
        std::cout << "B " << (unsigned long)Start << std::endl;
        std::cout << "E " << (unsigned long)Start+Size << std::endl;
        
        for (size_t i = 0; i < Size; i++)
        {
            std::cout << std::dec << (unsigned int)(*(Start+i)) << ", ";
        }
        std::cout << std::endl;

        std::cout << "next " << Right << std::endl;
        std::cout << std::endl;
    }



};
