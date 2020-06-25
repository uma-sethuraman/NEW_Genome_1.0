/**
 * \file SegmentNode.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include "SegmentNode.h"


/** Truncates Data from left 
 * \param cutSize amount to cut off the left of current segment 
 **/
void SegmentNode::TruncateLeft(size_t cutSize)
{
    Start = Start+cutSize;
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


/** Subdivide segment at index
 * \param index Index to cut the segment
 * \return pair of two new SegmentNodes to the cut segments 
 **/
SegmentNode* SegmentNode::Cut(size_t index)
{
    SegmentNode* cutSegment = new SegmentNode(Data, Start+index, Size-index);
    TruncateRight(Size-index);

    return cutSegment;
}

/** Gets data from Segment
 * \returns size of data **/
const Byte SegmentNode::getData(size_t index)
{
    if (index < Size)
        return *Start+index;
    else
    {
        std::cout << "Errpr: trying to get data from index " << (int)index << std::endl;
        print();
        exit(1);
    }
    
}

/** prints node info **/
void SegmentNode::print()
{
    std::cout << "SegmentNode " << this << std::endl;
    std::cout << "weight " << Weight << " size " << Size << " height " << Height << std::endl;
    std::cout << "left " << std::hex << Left << std::endl;
    std::cout << "B " << (unsigned long)Start << std::endl;
    std::cout << "E " << (unsigned long)Start+Size << std::endl;
    
    for (size_t i = 0; i < Size; i++)
    {
        std::cout << std::dec << (unsigned int)(*(Start+i)) << ", ";
    }
    std::cout << std::endl;

    std::cout << "right " << Right << std::endl;
    std::cout << std::endl;
}
