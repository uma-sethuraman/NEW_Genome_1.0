/**
 * \file SegmentList.cpp
 * \author Victoria Cao
 **/

#define pageSize 3000
#define mutationRate 33 // total all mutations 1 in every 100 sites


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
    size_t poolSize = std::max((size_t)10, (size_t)(std::ceil(size/pageSize)+std::ceil(size/mutationRate)));
    Pool = new SegmentPool(poolSize);

    // Creates list
    size_t node = 0;
    size_t start = 0;

    while(start < size)
    {
        // creates new node
        size_t nodeSize = std::min((size_t)pageSize, size - start);
        size_t newNode = Pool->Allocate(std::make_shared< std::vector<Byte> >(nodeSize));

        // updates root if at start 
        if (start == 0)
            Root = newNode;

        // adds node to indexing table
        IndexTable.push_back( {(size_t)(start), (size_t)(newNode)} );   

        // links node
        Pool->SetNext(node, newNode);
        node = newNode;
        start += nodeSize;
    }

}

/** 
 * Copy Constructor 
 * \param list to copy from
 **/
SegmentList::SegmentList(const SegmentList &list)
    : Pool(new SegmentPool(*(list.Pool))), IndexTable(list.IndexTable), Root(list.Root), SiteCount(list.SiteCount) 
{
}


/** 
 * Reallocates the current list to a new list
 * \returns pointer to newly created list
 **/
void SegmentList::Reallocate()
{
    SegmentPool* oldPool = Pool;
    size_t oldNode = Root;
    size_t oldOffset = 0;

    Pool = new SegmentPool(SiteCount);
    IndexTable.clear();
    IndexTable.resize(0);

    size_t node = 0;
    size_t start = 0;

    while(start < SiteCount)
    {
        size_t nodeSize = std::min((size_t)pageSize, SiteCount - start);
        size_t newNode = Pool->Allocate(std::make_shared< std::vector<Byte> >(nodeSize));
        if (start == 0)
            Root = newNode;

        // copy data from old list to new list
        size_t newlyAllocated = 0;
        IndexTable.push_back( {(size_t)(start), (size_t)(newNode)} );   

        while(newlyAllocated < nodeSize)
        {
            size_t moveSize = std::min({oldPool->GetSize(oldNode)-oldOffset, pageSize-newlyAllocated, SiteCount-start});

            Pool->Overwrite(newNode, newlyAllocated, oldPool->GetData(oldNode, oldOffset), moveSize);

            newlyAllocated += moveSize;
            oldOffset += moveSize;
            start += moveSize;

            if (oldOffset >= oldPool->GetSize(oldNode))
            {
                oldOffset = 0;
                oldNode = oldPool->GetNext(oldNode);
            }
        }

        Pool->SetNext(node, newNode);
        node = newNode;
    }

    delete oldPool;
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
 * \return pair node index in pool and offset within node
 **/
TableEntry SegmentList::Find(size_t index)
{
    // binary sesarch through Index Table 
    auto entry = std::lower_bound(IndexTable.begin(), IndexTable.end(), std::make_pair<size_t, size_t>((size_t)index, 0))-IndexTable.begin();

    if (entry >= IndexTable.size() || (IndexTable[entry].first != index && entry > 0))
    {
        --entry;
    }

    auto found = IndexTable[entry];
    size_t node = found.second;
    size_t left = found.first;

    // iterate through segment list if not in node
    while (left + Pool->GetSize(node)-1 < index && left + Pool->GetSize(node) < SiteCount)
    {
        left += Pool->GetSize(node);
        node = Pool->GetNext(node);

        // update index table
        IndexTable.push_back(std::make_pair<size_t, size_t>((size_t)(left), (size_t(node))));
        entry = IndexTable.size()-1;
    }

    return TableEntry(entry, node, left);
}

/** 
 * Gets Byte poitner to data
 * \param index in genome
 * \return pointer to data
 **/
Byte* SegmentList::GetData(size_t index)
{
    auto found = Find(index);
    return Pool->GetData(found.PoolIndex, index-found.Offset);
}


/** 
 * Overwrites into index of list
 * \param index 
 * \param segment to overwrite with
 **/
void SegmentList::Overwrite(size_t index, const std::vector<Byte>& segment)
{
    auto found = Find(index);
    auto left = found.PoolIndex;
    auto leftOffset = index-found.Offset;
    auto entry = ++found.Entry;

    size_t startIndex = 0;

    while(startIndex < segment.size())
    {
        // overwrite the node if current genome is only one with data
        if (Pool->Unique(left))
        {
            size_t size = std::min((Pool->GetSize(left)-leftOffset), (segment.size()-startIndex));
   
            auto ele = &(*segment.begin()); 
            Pool->Overwrite(left, leftOffset, segment, startIndex, size);
            startIndex += size;
            leftOffset = 0;
            left = Pool->GetNext(left);      
        }

        // create a new node to replace
        else
        {      
            auto right = left;
            auto rightOffset = leftOffset;
            auto tempIndex = startIndex;

            while(!Pool->Unique(right) && startIndex < segment.size())
            {
                // remove end 
                if (startIndex + Pool->GetSize(right) - rightOffset > segment.size())
                {
                    rightOffset = leftOffset + segment.size() - startIndex;

                    if (left == right)
                    {
                        if (leftOffset)
                        {
                            right = Pool->Copy(left);
                            Pool->TruncateRight(left, leftOffset);
                        }
                        else
                            left = Pool->GetPrev(left);
                    }

                    Pool->TruncateLeft(right, rightOffset);
                    startIndex = segment.size();
                }

                // remove rest of node
                else 
                {
                    // if not whole node
                    if (rightOffset)
                    {
                        startIndex += Pool->GetSize(right)-rightOffset;
                        Pool->TruncateRight(right, rightOffset);
                    }

                    else
                    {
                        startIndex += Pool->GetSize(right);
                        Pool->Clear(right);
                        
                    }

                    right = Pool->GetNext(right); 
                    rightOffset = 0; 
                    leftOffset = 0;
                }
            }
            // insert segment
            size_t mutation = Pool->Allocate(std::make_shared< std::vector<Byte> >(segment.begin()+tempIndex, segment.begin()+startIndex));
            
            // move left node if removed
            if (!Pool->Allocated(left))
            {
                left = Pool->GetPrev(left);

                // set new root if needed
                if (!left)
                {
                    Root = mutation;
                    IndexTable[0].second = (size_t)(Root);
                }
            }
            
            Pool->SetNext(left, mutation);
            Pool->SetNext(mutation, right);

            left = right;
            leftOffset = rightOffset;

            IndexTable.resize(entry);
        }
    }


}

/** 
 * Inserts into list
 * \param index 
 * \param segment to insert
 **/
void SegmentList::Insert(size_t index, const std::vector<Byte>& segment)
{
    auto found = Find(index);
    auto left = found.PoolIndex;
    auto leftOffset = index-found.Offset;
    auto entry = ++found.Entry;

    // overwrite current genome
    if (Pool->Unique(left))
    {
        if (Pool->GetSize(left) > 4098)
        {
            size_t cut = Pool->Split(left);
            if (Pool->GetSize(left) < leftOffset)
            {
                leftOffset -= Pool->GetSize(left);
                left = cut;
            }
        }

        Pool->Insert(left, leftOffset, segment);
    }

    // insert in between segments
    else
    {
        // create new mutation
        size_t mutation = Pool->Allocate(std::make_shared< std::vector<Byte> >(segment));
        size_t right = left;

        // in middle
        if (leftOffset && leftOffset != Pool->GetSize(left))
        {
            right = Pool->Copy(left);

            Pool->TruncateRight(left, leftOffset);
            Pool->TruncateLeft(right, leftOffset); 

            Pool->SetNext(left, mutation);

        }

        // front case
        else if (!leftOffset)
        {
            if (left != Root)
            {
                left = Pool->GetPrev(left);
                Pool->SetNext(left, mutation);
                IndexTable[--entry].second = mutation;
            }
            else
            {
                Root = mutation;
                IndexTable[0].second = (size_t)(Root);
            }


        }

        // back case
        else
        {
            right = Pool->GetNext(right);
            Pool->SetNext(left, mutation);
        } 
        
        Pool->SetNext(mutation, right);
    }

    SiteCount += segment.size();
    IndexTable.resize(entry);
}


/** 
 * Inserts into list
 * \param index 
 * \param segment to insert
 **/
void SegmentList::Remove(size_t index, size_t segmentSize)
{
    auto found = Find(index);
    auto left = found.PoolIndex;
    auto leftOffset = index-found.Offset;
    auto entry = ++found.Entry;

    auto right = left;
    auto rightOffset = leftOffset;

    SiteCount -= segmentSize;

    while(segmentSize)
    {
        // move rightOffset
        if (Pool->GetSize(right)-rightOffset > segmentSize)
        {
            // cut 
            if (Pool->Unique(right))
            {
                Pool->Remove(right, rightOffset, segmentSize);

                if (left == right)
                    right = Pool->GetNext(right);
            }

            else      
            {
                rightOffset = leftOffset+segmentSize;
                if (left == right)
                {
                    if (leftOffset)
                    {
                        right = Pool->Copy(left);
                        Pool->TruncateRight(left, leftOffset);
                    }
                    else
                        left = Pool->GetPrev(left);
                }
                Pool->TruncateLeft(right, rightOffset);  
            }          

            segmentSize = 0;
        }

        // remove part of node/whole node
        else 
        {
            // remove part of node
            if (rightOffset)
            {
                auto removeSize = std::min((long)(Pool->GetSize(right)-rightOffset), (long)segmentSize);
                
                if (Pool->Unique(right))
                    Pool->Remove(right, rightOffset, removeSize);

                else
                    Pool->TruncateRight(right, rightOffset);

                segmentSize -= removeSize; 
            }

            // remove whole node
            else
            {
                segmentSize -= Pool->GetSize(right);
                Pool->Clear(right);
            }

            right = Pool->GetNext(right); 
            rightOffset = 0;  
            leftOffset = 0;         
        } 
    }

    // move left node if removed
    if (!Pool->Allocated(left))
    {
        left = Pool->GetPrev(left);

        // set new root if needed
        if (!left)
        {
            Root = right;
            IndexTable[0].second = (size_t)(right);
        }
    }
        
    Pool->SetNext(left, right);
    IndexTable.resize(entry);
}

/** 
 * Prints the segment list
 **/
void SegmentList::Print()
{
    // print index table
    std::cout << CYAN;
    for (const auto &entry : IndexTable)
    {
        std::cout << entry.first << ", " << entry.second << std::endl;
    }

    std::cout << RESET << std::endl;

    
    // print segment list
    size_t node = Root;
    while (node)
    {
        Pool->Print(node);
        node = Pool->GetNext(node);
    }
}
