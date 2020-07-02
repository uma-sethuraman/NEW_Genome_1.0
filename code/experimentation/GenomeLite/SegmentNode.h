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

class SegmentPool;

/** Implementation of a genome object **/
class SegmentNode
{
private:
    // friend class SegmentTree;
    friend class GenomeLite;

    /// Data variables
    std::shared_ptr< GeneSegment > Data;
	Byte* Start; ///< Start of memory
    size_t Size; ///< Size of data

    /// Linked list variables
    size_t Left = std::numeric_limits<size_t>::max();
    size_t Right = std::numeric_limits<size_t>::max();
    size_t Parent = std::numeric_limits<size_t>::max();

    int Height = 0;
    size_t Weight = 0;
    size_t Pos;

public:
    /** Default Constructor */
    SegmentNode() = default;

    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< GeneSegment> data, size_t pos) 
        : Data(data), Start(data->GetData()), Size(data->GetSize()), Weight(data->GetSize()), Pos(pos) {}


    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< GeneSegment> data, size_t pos, Byte* start, size_t size) 
        : Data(data), Start(start), Size(size), Weight(size), Pos(pos) {}

    /** Deconstructor **/
    ~SegmentNode() { }

    /** Gets size
     * \returns size of data **/
    const size_t GetPos() { return Pos; }

    /** Gets size
     * \returns size of data **/
    const size_t GetSize() { return Size; }

    /** Gets size
     * \returns size of data **/
    const size_t GetWeight() { return Weight; }

    /** Gets height
     * \returns height of node**/
    const size_t GetHeight() { return Height; }


    const Byte GetData(size_t index);
    SegmentNode* GetLeft(SegmentPool* pool);
    SegmentNode* GetRight(SegmentPool* pool);
    SegmentNode* GetParent(SegmentPool* pool);

    /** Gets size
     * \returns size of data **/
    void SetWeight(size_t weight) { Weight = weight; }

    /** Gets size
     * \returns size of data **/
    void SetHeight(int height) { Height = height; }


    void SetLeft(SegmentNode* node);
    void SetRight(SegmentNode* node);
    void SetParent(SegmentNode* node);


    void TruncateLeft(size_t cutSize = 1);
    void TruncateRight(size_t cutSize = 1);
    SegmentNode* Cut(SegmentPool* pool, size_t index);
    void Print();

};
