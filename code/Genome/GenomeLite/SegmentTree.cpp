/**
 * \file SegmentTree.cpp
 * \author Victoria Cao
 **/

#include <algorithm>
#include <iostream>
#include <queue>

#include "SegmentTree.h"
#include "../../utilities.h"



/** Constructor 
 * \param size **/
SegmentTree::SegmentTree(size_t size) 
    : NodePool(new MemoryPool< SegmentNode >(size*(3/4)))
{
    Root = CreateNode();
    Root->Leaf = false;

    auto newNode = CreateNode();
    newNode->Parent = Root->Position;

    Root->Children[0] = newNode->Position;
    Root->Size = 1;
}

SegmentNode* SegmentTree::CreateNode()
{
    auto newNode = NodePool->Allocate();
    newNode->Position = NodePool->GetTail()-1;

    return newNode;
}


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
        auto nodeIndex = node->UpperBound(index);

        node = NodePool->At(node->Children[nodeIndex]);
        if (nodeIndex)
            LeftBound += node->Keys[nodeIndex-1].Size;
    }

    return {node, LeftBound};
}

/** Inserts Gene segment at index
 * \param segment to insert
 * \param index to insert at
 **/
void SegmentTree::Insert(size_t index, std::vector<std::byte> segment)
{
    auto foundPair = Find(index);
    auto node = foundPair.first;
    GeneSegment newSegment = std::make_shared< std::vector<Byte> >(segment);

    // insert into non-full node
    if (node->Size < NODE_SIZE)
    {
        
        auto offset = index - foundPair.second;
        node->Insert(offset, newSegment);
    }

    // inserting into full node
    else
    {
        /* code */
    }
    


    // update tree
    ++Size;
    SiteCount += segment.size();
}

/** Prints the tree **/
void SegmentTree::Print()
{
    std::cout << std::endl;
    std::queue<SegmentNode*> q;
    q.push(Root);

    while(q.size() > 0)
    {
        auto node = q.front();
        q.pop();

        std::cout << "\nNode: " << node << std::endl;
        std::cout << "Weight" << node->Weight << "\tPos: " << node->Position << std::endl;
        
        
        std::cout << "Keys: ";
        for (size_t i(0); i < node->Size; ++i)
        {
            if (node->Leaf)
                std::cout << "Gene: " << node->Keys[i-1].Gene.get();
            std::cout << " Size" << node->Keys[i-1].Size << "\t";
        }
        
        
        std::cout << "\nChildren: ";

        for (size_t i(0); i < node->Size; i++)
        {
            std::cout << node->Children[i] << "\t";
            q.push(NodePool->At(node->Children[i]));
        }

        std::cout << std::endl;
    }
    std::cout << std::endl;

}