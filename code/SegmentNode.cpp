/**
 * \file SegmentNode.cpp
 * \author Victoria Cao
 **/

#include <iostream>
#include "SegmentNode.h"


/** Truncates Segment from left 
 * \param cutSize amount to cut off the left of current segment 
 **/
void SegmentNode::TruncateLeft(size_t cutSize)
{
    std::cout << "SegmentNode " << this << " Truncate Left " << cutSize << std::endl;
    Start = Start+cutSize;
    Size -= cutSize;

    
    std::cout << "B " << std::hex << (unsigned long)Start << std::endl;
    std::cout << "E " << (unsigned long)Start+Size << std::endl;
    std::cout << std::endl;
}

/** Truncates Segment from right 
 * \param cutSize amount to cut off the right of current segment 
 **/
void SegmentNode::TruncateRight(size_t cutSize)
{
    Size -= cutSize;

    std::cout << "SegmentNode " << this << " Truncate Right " << cutSize << std::endl;
    std::cout << "B " << std::hex << (unsigned long)Start << std::endl;
    std::cout << "E " << (unsigned long)Start+Size << std::endl;
    std::cout << std::endl;
}


/** Subdivide segment at index
 * \param index Index to cut the segment
 * \return pair of two new SegmentNodes to the cut segments 
 **/
std::shared_ptr<SegmentNode> SegmentNode::Cut(size_t index)
{
    auto cutSegment = std::make_shared<SegmentNode>(Segment, Start+index, Size-index);
    TruncateRight(Size-index);

    return cutSegment;
}

