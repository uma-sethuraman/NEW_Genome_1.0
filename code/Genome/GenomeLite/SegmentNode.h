/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief struct definition for a SegmentNode
 **/

#pragma once

#include <limits>
#include <memory>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cstddef>

#include "GeneSegment.h"

// define Byte
typedef std::byte Byte;

class MemoryPool;

/** Implementation of a genome object **/
class SegmentNode
{
private:
    // friend class SegmentTree;
    friend class GenomeLite;
    friend class SegmentTree;

    /// Data variables
    std::shared_ptr< GeneSegment > Gene;
    Byte* Start;
    size_t Size;


    /// Linked list variables
    size_t Left = 0;
    size_t Right = 0;
    size_t Parent = 0;

    int Height = 0;
    size_t Weight = 0;
    size_t Pos;

public:
    /** Default Constructor */
    SegmentNode() = default;

    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(size_t maxSize, size_t size, size_t pos) 
        : Gene(std::make_shared< GeneSegment >(maxSize, size)), Size(size), Weight(size), Pos(pos) 
    {
        Start = Gene->Gene;
    }

    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(size_t size, size_t pos) 
        : Gene(std::make_shared< GeneSegment >(size, size)), Size(size), Weight(size), Pos(pos)
    {
        Start = Gene->Gene;
    }


    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< GeneSegment> gene, size_t pos) 
        : Gene(gene), Start(gene->Gene), Size(gene->Size), Weight(gene->Size), Pos(pos) {}


    /** Constructor
     * \param start beginning of gene
     * \param size size of gene */
    SegmentNode(std::shared_ptr< GeneSegment> gene, size_t pos, Byte* start, size_t size) 
        : Gene(gene), Start(start), Size(size), Weight(size), Pos(pos) {}

    /** Deconstructor **/
    ~SegmentNode() {}

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

    Byte* GetData(size_t index);
    SegmentNode* GetLeft(MemoryPool* pool);
    SegmentNode* GetRight(MemoryPool* pool);
    SegmentNode* GetParent(MemoryPool* pool);

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
    void Delete(size_t index, size_t segmentSize);
    void Insert(size_t index, std::vector<Byte> segment);
    void Overwrite(size_t index, std::vector<Byte> segment);


    SegmentNode* Cut(MemoryPool* pool, size_t index);
    void Print();

};