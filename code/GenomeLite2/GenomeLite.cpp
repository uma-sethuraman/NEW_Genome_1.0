/**
 * \file GenomeLite.cpp
 * \author Victoria Cao
 **/


#include <deque>

#include "GenomeLite.h"

GenomeLite::GenomeLite(size_t size)
{
     
    
}

std::pair<SegmentNode*, size_t> GenomeLite::Find(size_t index)
{
    SegmentNode* node = Root;
    size_t left = 0;    // Left boundary of node        

    while (!node->Leaf)
    {
        size_t i = 0;
        while (left+node->Keys[i].Size <= index)
        {
            left += node->Keys[i].Size;
            ++i;
        }     

        node = node->Child[i];

    }

    return {node, left};
}



void GenomeLite::SplitChild(SegmentNode* node)
{

}


void GenomeLite::overwrite(size_t index, const std::vector< Byte > &segment)
{

}


