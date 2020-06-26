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
    friend class GenomeLite;

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
    /** Default Constructor */
    SegmentNode() = default;

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
    const size_t GetSize() { return Size; }

    /** Gets height
     * \returns height of node**/
    const size_t GetHeight() { return Height; }


    const Byte GetData(size_t index);
    void TruncateLeft(size_t cutSize = 1);
    void TruncateRight(size_t cutSize = 1);
    SegmentNode* Cut(size_t index);
    void Print();

};
