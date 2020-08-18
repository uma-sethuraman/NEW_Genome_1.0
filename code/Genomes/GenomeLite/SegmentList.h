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


typedef std::byte Byte; // easy reading

/** struct for finding **/
struct TableEntry
{
    size_t Index;
    size_t Size;
    Byte* Address;

    TableEntry(size_t index)
        : Index(index) {}

    TableEntry(size_t index, size_t size, Byte* address)
        : Index(index), Size(size), Address(address) {}
};


/** basic implementation of rope **/
class SegmentList
{
private:
    /// member variables
    std::vector< TableEntry > IndexTable; ///< pair(index and size)
    std::vector< Byte > Pool; //< byte pointers

    size_t SiteCount = 0;

public:
    SegmentList() = default;
    SegmentList(size_t size);
    SegmentList(const SegmentList &List);
    SegmentList* Reallocate();

    ~SegmentList() {}
    
    size_t GetSiteCount() { return SiteCount; }
    bool IsFull() { IndexTable.size()+3 > IndexTable.capacity(); }

    size_t Find(size_t index);
    Byte* GetData(size_t index, size_t segmentSize = 1);
    void Resize(size_t size);

    void Overwrite(size_t index, const std::vector<std::byte>& segment);
    void Insert(size_t index, const std::vector<std::byte>& segment);
    void Remove(size_t index, size_t segmentSize);
    void Print();    

};