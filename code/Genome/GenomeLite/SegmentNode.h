/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief struct definition for a SegmentNode
 **/

#pragma once

#define NODE_SIZE 4

#include <limits>
#include <memory>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cstddef>

// define Byte
typedef std::byte Byte;

/** struct for Byte array pair **/
struct GeneSegment
{
    std::shared_ptr< std::vector<Byte> > Gene;
    size_t Size = 0;
    size_t Start = 0;

    GeneSegment(std::shared_ptr< std::vector<Byte> > gene)
        : Gene(gene), Size(gene->size()) {}

    GeneSegment(size_t size) 
        : Size(size) {}

    GeneSegment() {}
};


/** struct for a SegmentNode **/
class SegmentNode
{
private:
    // variables
    GeneSegment Keys[NODE_SIZE-1];
    size_t Children[NODE_SIZE];

    bool Leaf = true;
    size_t Weight = 0; /// weight of all sums of nodes to left
    size_t Size = 0;

    size_t Parent;  /// parent location in memory pool
    size_t Position = 0; 

public:
    size_t UpperBound(size_t num)
    {
        size_t left = 0;
        size_t right = Size;

        while (left < right)
        {
            size_t mid = left + (right-1) /2;
            if (num > Keys[mid].Size)
                left = mid+1;
            else
                right = mid;
        }

        return left;
    }

    void Insert(size_t offset, GeneSegment segment)
    {
        size_t leftBound = 0;
        size_t index = 0
        // find position to insert
        while(leftBound + Keys[index].Size < offset)
        {            
            leftBound += Keys[i].Size;
            ++index;
        }
        
        // only this genome is using it, so directly edit gene
        if (Keys[index].Gene->use_count() == 1)
        {
            if (Keys[index].Size != Keys[index].Gene->size())
            {
                
            }
        }

        else
        {
            // in between case
            if (offset - leftBound > 0)
            {

            }
            else
            {

            }
        }
        




        
        
    }

    friend class SegmentTree;
};
