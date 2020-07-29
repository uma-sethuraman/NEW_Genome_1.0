/**
 * \file SegmentList.cpp
 * \author Victoria Cao
 **/

#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

#include "SegmentList.h"

#define RESET   "\033[0m"
#define CYAN    "\033[36m"      /* Cyan */

/** 
 * Constructor 
 * \param size of new list
 **/
SegmentList::SegmentList(size_t size)
    : SiteCount(size)
{
    auto mutations = size;
    Pool.reserve(size+mutations);
    Pool.resize(size);

    IndexTable.reserve(1+mutations);
    IndexTable.push_back( TableEntry(0, size, Pool.data()) );
}

/** 
 * Copy Constructor 
 * \param list to copy from
 **/
SegmentList::SegmentList(const SegmentList &list)
    : SiteCount(list.SiteCount)
{
    Pool = list.Pool;
    IndexTable = list.IndexTable;
}

/** 
 * Reallocates list
 * \returns new segmentList
 **/
SegmentList* SegmentList::Reallocate()
{
    auto newList = new SegmentList();
    newList->SiteCount = SiteCount;

    auto mutations = SiteCount;
    newList->Pool.reserve(SiteCount+mutations);
    newList->Pool.resize(SiteCount);

    newList->IndexTable.reserve(1+mutations);
    newList->IndexTable.push_back( TableEntry(0, SiteCount, newList->Pool.data()) );

    auto offset = 0;
    for (size_t i(0); i < IndexTable.size(); i++)
    {
        std::memcpy(newList->Pool.data()+offset, IndexTable.at(i).Address, IndexTable.at(i).Size);
        offset += IndexTable.at(i).Size;
    }

    return newList;
}

/**
 * Resizes the genome
 * \param size to resize to
 **/
void SegmentList::Resize(size_t size)
{
    if (size < SiteCount)
    {
        Remove(size, SiteCount-size);
    }
    else
    {
        Insert(SiteCount, std::vector< Byte >(size-SiteCount));
    }

    SiteCount = size;
}

/** 
 * Finds index in list
 * \param index to find
 * \return index into IndexTable
 **/
size_t SegmentList::Find(size_t index)
{
    // binary sesarch through Index Table 
    auto it = std::upper_bound(IndexTable.begin(), IndexTable.end(), TableEntry(index), [](const TableEntry& lhs, const TableEntry& rhs){ return lhs.Index < rhs.Index; }) ;
    return it - IndexTable.begin() - 1;
}


/** 
 * Gets Byte poitner to data
 * \param index in genome
 * \return pointer to data
 **/
Byte* SegmentList::GetData(size_t index, size_t segmentSize)
{
    auto tableIndex = Find(index);
    auto offset = index-IndexTable.at(tableIndex).Index;

    return IndexTable.at(tableIndex).Address+offset;
}


/** 
 * Overwrites into index of list
 * \param index 
 * \param segment to overwrite with
 **/
void SegmentList::Overwrite(size_t index, const std::vector<Byte>& segment)
{
    auto tableIndex = Find(index);
    size_t offset = index-IndexTable.at(tableIndex).Index;
    size_t overwritten = 0;

    // loop until overwrite is all written
    while (overwritten < segment.size())
    {
        auto copySize = std::min(segment.size()-overwritten, IndexTable.at(tableIndex).Size-offset);
        std::memcpy(IndexTable.at(tableIndex).Address+offset, segment.data()+overwritten, copySize);
        overwritten += copySize;

        ++tableIndex;
        offset = 0;
    }
}


/** 
 * Inserts into list
 * \param index 
 * \param segment to insert
 **/
void SegmentList::Insert(size_t index, const std::vector<Byte>& segment)
{
    auto tableIndex = Find(index);
    auto offset = index-IndexTable.at(tableIndex).Index;

    // update size
    SiteCount += segment.size();

    if (offset && offset != IndexTable.at(tableIndex).Size) // middle
    {        
        IndexTable.insert(IndexTable.begin()+tableIndex+1, TableEntry(IndexTable.at(tableIndex).Index+offset, IndexTable.at(tableIndex).Size-offset, IndexTable.at(tableIndex).Address+offset));
        IndexTable.at(tableIndex).Size = offset;
    }
    tableIndex += bool(offset); 

    // create mutation and insert into table
    IndexTable.insert(IndexTable.begin()+tableIndex, TableEntry(index, segment.size(), Pool.data()+Pool.size()));
    Pool.insert(Pool.end(), segment.begin(), segment.end());
        
    
    std::for_each(IndexTable.begin()+tableIndex+1, IndexTable.end(),[segment](TableEntry &entry)
    {
        entry.Index += segment.size();
    });
}


/** 
 * Inserts into list
 * \param index 
 * \param segmentSize to remove
 **/
void SegmentList::Remove(size_t index, size_t segmentSize)
{
    auto tableIndex = Find(index);
    auto offset = index-IndexTable.at(tableIndex).Index;

    // update size
    SiteCount -= segmentSize;

    size_t removed = 0;
    
    // loop until overwrite is all written
    while (removed < segmentSize)
    {
        IndexTable.at(tableIndex).Index -= removed;

        auto removeSize = std::min(segmentSize-removed, IndexTable.at(tableIndex).Size-offset);
        auto endOffset = offset+removeSize;

        // erase whole entry
        if (!offset && endOffset == IndexTable.at(tableIndex).Size)
        {
            IndexTable.erase(IndexTable.begin()+tableIndex);
            --tableIndex;
        }
        // edit current entry
        else
        {  
            if (endOffset == IndexTable.at(tableIndex).Size) // back
            {
                IndexTable.at(tableIndex).Size -= removeSize;
            }
            else if (offset) // middle
            {
                IndexTable.insert(IndexTable.begin()+tableIndex+1, TableEntry(index, IndexTable.at(tableIndex).Size-endOffset, IndexTable.at(tableIndex).Address+endOffset));
                IndexTable.at(tableIndex).Size = offset;
                
                ++tableIndex;
            }
            else // front
            {
                IndexTable.at(tableIndex).Address += removeSize;
                IndexTable.at(tableIndex).Size -= removeSize;
            }
        }
        
        removed += removeSize;
        offset = 0;

        tableIndex += bool(removed < segmentSize);
    }

    std::for_each(IndexTable.begin()+tableIndex+1, IndexTable.end(),[segmentSize](TableEntry &entry)
    {
        entry.Index -= segmentSize;
    });
}

/** 
 * Prints the segment list
 **/
void SegmentList::Print()
{
    // print index table
    std::cout << CYAN << "\nIndex Table:" << std::endl;;
    for (const auto &entry : IndexTable)
    {
        std::cout << entry.Index << "," << entry.Size << "," << entry.Address << std::endl;
    }

    std::cout << "\n" << RESET << std::endl;

    
    // print segment list
    for (const auto &entry : IndexTable)
    {
        for (size_t i(0); i < entry.Size; i++)
        {
            std::cout << (int)*(entry.Address+i) << ",";
        }
    }

    // std::cout << "\n" << std::endl;

    // for (const auto &segment : Pool)
    // {
    //     std::cout << (int)segment << ",";

    // }
    std::cout << std::endl;
}
