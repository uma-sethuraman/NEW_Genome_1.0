/**
 * \file SegmentNode.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include "SegmentNode.h"

#include "SegmentPool.h"

/** Gets data from Segment
 * \returns size of data **/
Byte* SegmentNode::GetData(size_t index)
{
    if (index < Size)
        return &(Data->at(Start+index));
    else
    {
        std::cout << "Error: trying to get data from index " << (int)index << std::endl;
        Print();
        exit(1);
    }
    
}


/** Gets left node reference
 * \returns node reference **/
SegmentNode* SegmentNode::GetPrev(SegmentPool* pool)
{
    if (Prev > 0)
        return pool->At(Prev);
    return nullptr;
}


/** Gets right node referencet
 * \returns node reference **/
SegmentNode* SegmentNode::GetNext(SegmentPool* pool)
{
    if (Next > 0)
        return pool->At(Next);
    return nullptr;
}


/** Sets Prev node
 * \param node **/
void SegmentNode::SetPrev(SegmentNode* node)
{
    if (node)
    {
        Prev = node->GetPos();
        node->Next = Pos;
    }
    else
        Prev = -1;
}

/** Sets Next node
 * \param node **/
void SegmentNode::SetNext(SegmentNode* node)
{
    if (node)
    {
        Next = node->GetPos();
        node->Prev = Pos;
    }
    else
        Next = -1;
}


/** Truncates Data from left 
 * \param index amount to cut off the left of current segment 
 **/
void SegmentNode::TruncateLeft(size_t index)
{
    Start += index;
    Size -= index;
}

/** Truncates Data from right 
 * \param index amount to cut off the right of current segment 
 **/
void SegmentNode::TruncateRight(size_t index)
{
    Size = index;
}


/** Subdivide segment at index
 * \param pool The pool of memory to make a pointer from
 * \return pair of two new SegmentNodes to the cut segments 
 **/
SegmentNode* SegmentNode::Copy(SegmentPool* pool)
{
    return pool->Allocate(Data, Start, Size);
}


size_t SegmentNode::Overwrite(size_t index, const std::vector<std::byte>& segment, size_t start)
{
    auto size = Size-index;
    std::memcpy(&(Data->at(index)), &(segment.at(start)), size);

    return start+size;
}


/** prints node info **/
void SegmentNode::Print()
{
    std::cout << "SegmentNode " << this << "\tsize " << Size << "\tpos " << Pos << std::endl;
    std::cout << "left " << Prev << std::endl;
    std::cout << "B " << (unsigned long)Start << std::endl;
    std::cout << "E " << (unsigned long)Start+Size << std::endl;
    
    for (size_t i = 0; i < Size; i++)
    {
        std::cout << std::dec << (int)Data->at(Start+i) << ", ";
    }
    std::cout << std::endl;

    std::cout << "right " << Next << std::endl;
    std::cout << std::endl;
}