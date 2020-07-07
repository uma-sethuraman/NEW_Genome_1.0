/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief struct definition for a SegmentNode
 **/

#pragma once

#include <cstdint>
#include <vector>

// define Byte
typedef char Byte;
// define 
#define N 4

/** struct for Byte array pair **/
struct GeneSegment
{
    Byte* Gene;
    size_t Size;
};

/** struct for a SegmentNode **/
class SegmentNode
{
private:
    // variables
    static std::vector< GeneSegment > Keys;
    static std::vector< SegmentNode* > Children;

    bool Leaf = true;
    size_t Size = 0; /// number of items in node
    size_t Weight = 0; /// weight of all sums of nodes to left

    size_t Parent;  /// parent location in memory pool

public:
    /** default constructor **/
    SegmentNode() 
    {
        Keys.reserve(N-1);
        Children.reserve(N);
    }


friend class SegmentTree;
};
