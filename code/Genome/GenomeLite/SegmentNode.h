/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief struct definition for a SegmentNode
 **/

#pragma once
#include <cstdint>

// define Byte
typedef char Byte;
// define 
#define N 10

/** struct for Byte array pair **/
struct GeneSegment
{
    Byte* Gene;
    size_t size;
};

/** struct for a SegmentNode **/
struct SegmentNode
{
    // variables
    static GeneSegment Keys[N - 1];
    static SegmentNode Children[N];

    bool Leaf = true;
    int Size = 0; /// number of items in node

    size_t Parent;  /// parent location in memory pool
};
