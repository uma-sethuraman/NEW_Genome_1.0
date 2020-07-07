/**
 * \file SegmentTree.cpp
 * \author Victoria Cao
 **/

#include <algorithm>

#include "SegmentTree.h"


/** Splits the child node
 * \param index to split at
 **/
void SegmentTree::SplitChild(SegmentNode* node, size_t index)
{

}

/** Finds the index 
 * \returns SegmentNode that index is within
 * and the starting index of the node **/
std::pair<SegmentNode*, size_t> SegmentTree::Find(size_t index)
{
    SegmentNode* node = Root;
    size_t LeftBound = 0;

    // find leaf node with index
    while(!node->Leaf)
    {
        size_t nodeIndex = std::upper_bound(node->Keys.begin(), node->Keys.end(), index) - node->Keys.begin();
        node = node->Children[nodeIndex];
        if (nodeIndex)
            LeftBound += node->Keys[nodeIndex-1].Size;
    }

    // find segment in leaf node
    for(size_t i(0); i < node->Size; i++)
    {
        if (LeftBound + node->Keys[i].Size < index)
            LeftBound += node->Keys[i].Size;
        else
            return {node, LeftBound};
    }
}

/** Inserts Gene segment at index
 * \param segment to insert
 * \param index to insert at
 **/
void SegmentTree::Insert(GeneSegment &segment, size_t index)
{
    auto foundPair = Find(index);
    if (foundPair.first)
    {

    }
    else
    {
        
    }

    // update tree
    ++Size;
    SiteCount += segment.Size;
}