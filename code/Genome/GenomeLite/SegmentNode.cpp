/**
 * \file SegmentNode.cpp
 * \author Victoria Cao
 **/

#include <iostream>

#include "SegmentNode.h"
#include "MemoryPool.h"


/** Gets data from Segment
 * \returns size of data **/
Byte* SegmentNode::GetData(size_t index)
{
    if (index < Size)
        return Start+index;
    else
    {
        std::cout << "Error: trying to get data from index " << (int)index << std::endl;
        Print();
        exit(1);
    }
    
}


/** Gets left node reference
 * \returns node reference **/
SegmentNode* SegmentNode::GetLeft(MemoryPool* pool)
{
    if (!Left)
        return nullptr;
    return pool->At(Left);
}


/** Gets right node referencet
 * \returns node reference **/
SegmentNode* SegmentNode::GetRight(MemoryPool* pool)
{
    if (!Right)
        return nullptr;
    return pool->At(Right);
}


/** Gets right node referencet
 * \returns node reference **/
SegmentNode* SegmentNode::GetParent(MemoryPool* pool)
{
    if (!Parent)
        return nullptr;
    return pool->At(Parent);
}

/** Sets Left node
 * \param node **/
void SegmentNode::SetLeft(SegmentNode* node)
{
    if (node)
        Left = node->GetPos();
    else
        Left = 0;
}

/** Sets Right node
 * \param node **/
void SegmentNode::SetRight(SegmentNode* node)
{
    if (node)
        Right = node->GetPos();
    else
        Right = 0;
}

/** Sets Parent node
 * \param node **/
void SegmentNode::SetParent(SegmentNode* node)
{
    if (node)
        Parent = node->GetPos();
    else
        Parent = 0;
}


/** Truncates Data from left 
 * \param cutSize amount to cut off the left of current segment 
 **/
void SegmentNode::TruncateLeft(size_t cutSize)
{
    Start += cutSize;
    Size -= cutSize;
    Weight -= (int)cutSize;
}

/** Truncates Data from right 
 * \param cutSize amount to cut off the right of current segment 
 **/
void SegmentNode::TruncateRight(size_t cutSize)
{
    Size -= cutSize;
    Weight -= (int)cutSize;
}

/** 
 * \param index 
 **/
void SegmentNode::Delete(size_t index, size_t segmentSize)
{

}


/** 
 * \param index 
 **/
void SegmentNode::Insert(size_t index, std::vector<Byte> segment)
{

}

/** 
 * \param index 
 **/
void SegmentNode::Overwrite(size_t index, std::vector<Byte> segment)
{
    for (size_t i(0); i < segment.size(); i++)
    {
        *(Start+index+i) = segment[i];
    }
}


/** Subdivide segment at index
 * \param pool The pool of memory to make a pointer from
 * \param index Index to cut the segment
 * \return pair of two new SegmentNodes to the cut segments 
 **/
SegmentNode* SegmentNode::Cut(MemoryPool* pool, size_t index)
{
    SegmentNode* cutSegment = pool->Allocate(Gene, Start+index, Size-index);
    TruncateRight(Size-index);

    return cutSegment;
}



/** prints node info **/
void SegmentNode::Print()
{
    std::cout << "SegmentNode " << this << std::endl;
    std::cout << "weight " << std::dec << Weight << " size " << Size << " height " << Height << std::endl;
    std::cout << "left " << Left << std::endl;
    std::cout << "B " << Start << std::endl;
    std::cout << "E " << Start+Size << std::endl;
    
    for (size_t i = 0; i < Size; i++)
    {
        std::cout << std::dec << (int)(*(Start+i)) << ", ";
    }
    std::cout << std::endl;

    std::cout << "right " << Right << std::endl;
    std::cout << std::endl;
}