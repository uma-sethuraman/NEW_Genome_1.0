/**
 * \file SegmentList.h
 * \author Victoria Cao
 * 
 * \brief class for a rope data structure
 **/

#pragma once

#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <queue>
#include <cmath>

#include "SegmentPool.h"
#include "SegmentNode.h"

typedef std::byte Byte; // easy reading

/** basic implementation of rope **/
class SegmentList
{
private:
    friend class GenomeLite;

    /// member variables
    SegmentPool* Pool; ///< allocation pool for nodes
    size_t Root; ///< Root node of tree

    size_t SiteCount = 0;

public:
    SegmentList(size_t size);
    SegmentList(const SegmentList &List);

    ~SegmentList() { delete Pool; }
    size_t GetSiteCount() { return SiteCount; }

    std::pair< size_t, size_t > Find(size_t index);
    std::pair< SegmentNode*, size_t > FindEnd(SegmentNode* node, size_t start, size_t size);
    Byte* GetData(size_t index);
    void Overwrite(size_t index, const std::vector<std::byte>& segment);
    void Insert(size_t index, const std::vector<std::byte>& segment);
    void Remove(size_t index, size_t segmentSize);
    void Print();    

};