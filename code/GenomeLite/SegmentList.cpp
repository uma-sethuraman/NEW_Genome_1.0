/**
 * \file SegmentList.cpp
 * \author Victoria Cao
 **/

#define pageSize 3000

#include <cstdint>
#include <vector>
#include <utility>
#include <algorithm>

#include "SegmentList.h"

#define RESET   "\033[0m"
#define CYAN    "\033[36m"      /* Cyan */

/** 
 * Constructor 
 * \param size of new list
 **/
SegmentList::SegmentList(size_t size)
    :Pool(new SegmentPool(size)), SiteCount(size)
{
    size_t node;
    size_t startSize = 0;

    if (size >= pageSize)
    {
        node = Pool->Allocate(std::make_shared< std::vector<Byte> >(pageSize));
        startSize += pageSize;
    }
    else
    {
        node = Pool->Allocate(std::make_shared< std::vector<Byte> >(size));
        startSize = size;
    }

    Root = node;
    IndexTable.push_back( {(size_t)0, (size_t)node} );


    size_t startNode = node;
    size_t newNode;

    while (startSize < size)
    {
        size_t lastSize = startSize;
        if (size-startSize >= pageSize)
        {
            newNode = Pool->Allocate(std::make_shared< std::vector<Byte> >(pageSize));
            startSize += pageSize;
        }
        else
        {
            newNode = Pool->Allocate(std::make_shared< std::vector<Byte> >(size-startSize));
            startSize = size;
        }
        Pool->SetNext(node, newNode);
        node = newNode;

        IndexTable.push_back( {(size_t)lastSize, (size_t)node} );
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
    size_t oldRoot = Root;

    Pool = new SegmentPool(SiteCount);
    // Root = CreateList(SiteCount, 0);
}

size_t SegmentList::CreateList(size_t size, size_t index)
{
}

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
}

/** 
 * Finds index in list
 * \param index to find
 * \return pair node index in pool and offset within node
 **/
TableEntry SegmentList::Find(size_t index)
{
    // binary sesarch through Index Table 
    auto entry = std::lower_bound(IndexTable.begin(), IndexTable.end(), std::make_pair<size_t, size_t>((size_t)index, 0));
    if ((entry != IndexTable.begin() && (*entry).first > index) || entry == IndexTable.end())
        --entry;
    
    size_t node = (*entry).second;
    size_t left = (*entry).first;
    auto tableIt = entry-IndexTable.begin();

    // iterate through segment list if not in node
    while (left + Pool->GetSize(node) < index)
    {
        left += Pool->GetSize(node);
        node = Pool->GetNext(node);

        // update index table
        IndexTable.push_back(std::make_pair<size_t, size_t>((size_t)(left), (size_t(node))));
        tableIt = IndexTable.size()-1;
    }

    return TableEntry(tableIt, node, left);
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
void SegmentList::Overwrite(size_t index, const std::vector<std::byte>& segment)
{
    auto found = Find(index);
    auto left = found.PoolIndex;
    auto leftOffset = index-found.Offset;
    auto tableEnd = ++found.TableEnd;

    size_t startIndex = 0;

    while(startIndex < segment.size())
    {
        // overwrite the node if current genome is only one with data
        if (Pool->Unique(left))
        {
            startIndex = Pool->Overwrite(left, leftOffset, segment, startIndex);
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

        }
    }

    IndexTable.resize(tableEnd);

}

/** 
 * Inserts into list
 * \param index 
 * \param segment to insert
 **/
void SegmentList::Insert(size_t index, const std::vector<std::byte>& segment)
{
    auto found = Find(index);
    auto left = found.PoolIndex;
    auto leftOffset = index-found.Offset;
    auto tableEnd = found.TableEnd;

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
            right = left;
            if (leftOffset == Pool->GetSize(left))
            {
                right = Pool->GetNext(right);
            }
            else
            {
                right = Pool->Copy(left);
                Pool->TruncateRight(left, leftOffset);
                Pool->TruncateLeft(right, leftOffset); 
            }
            Pool->SetNext(left, mutation);
        }

        // front case
        else if (!leftOffset)
        {
            if (left != Root)
                Pool->SetNext(left, mutation);
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
    
    if (++tableEnd < IndexTable.size())
        IndexTable.erase(IndexTable.begin()+tableEnd, IndexTable.end());

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
    auto tableEnd = found.TableEnd;

    auto right = left;
    auto rightOffset = leftOffset;

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
                rightOffset = segmentSize;
                if (left == right)
                {
                    right = Pool->Copy(left);
                    Pool->TruncateRight(left, leftOffset);
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

    SiteCount -= segmentSize;

    if (++tableEnd < IndexTable.size())
        IndexTable.erase(IndexTable.begin()+tableEnd, IndexTable.end());

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
