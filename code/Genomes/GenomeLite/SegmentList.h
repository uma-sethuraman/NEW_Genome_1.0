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
#include <vector>
#include <iterator>
#include <cmath>

#include "SegmentPool.h"
#include "SegmentNode.h"

typedef std::byte Byte; // easy reading

/** struct for finding **/
struct TableEntry
{
    size_t Entry;
    size_t PoolIndex;
    size_t Offset;

    TableEntry(size_t entry, size_t pInd, size_t offset)
        : Entry(entry), PoolIndex(pInd), Offset(offset) {}
};


/** basic implementation of rope **/
class SegmentList
{
private:
    friend class GenomeLite;

    /// member variables
    SegmentPool* Pool; ///< allocation pool for nodes
    std::vector< std::pair<size_t, size_t> > IndexTable; ///< pair(index and location in segmentPool)
    size_t Root; ///< Root node of tree

    size_t SiteCount = 0;

public:
    SegmentList() = delete;
    SegmentList(size_t size);
    SegmentList(const SegmentList &List);
    void Reallocate();

    ~SegmentList() { delete Pool; }

    const bool IsFull() { return Pool->IsFull(); }
    size_t GetSiteCount() { return SiteCount; }

    TableEntry Find(size_t index);
    Byte* GetData(size_t index);
    void Resize(size_t size);

    void Overwrite(size_t index, const std::vector<std::byte>& segment);
    void Insert(size_t index, const std::vector<std::byte>& segment);
    void Remove(size_t index, size_t segmentSize);
    void Print();    

};