/**
 * \file SegmentList.cpp
 * \author Victoria Cao
 **/

#define pageSize 20

#include <cstdint>
#include <vector>

#include "SegmentList.h"

SegmentList::SegmentList(size_t size)
    :Pool(new SegmentPool(size))
{
    if (size >= pageSize)
    {
        Root = Pool->Allocate(std::make_shared< std::vector<Byte> >(pageSize));
        size -= pageSize;
    }
    else
    {
        Root = Pool->Allocate(std::make_shared< std::vector<Byte> >(size));
        size = 0;
    }

    size_t node = Root;
    size_t newNode;
    while (size)
    {
        if (size >= pageSize)
        {
            newNode = Pool->Allocate(std::make_shared< std::vector<Byte> >(pageSize));
            size -= pageSize;
        }
        else
        {
            newNode = Pool->Allocate(std::make_shared< std::vector<Byte> >(size));
            size = 0;
        }
        Pool->SetNext(node, newNode);
        node = newNode;
    }
}

SegmentList::SegmentList(const SegmentList &list)
    : Pool(new SegmentPool(*(list.Pool))), Root(list.Root), SiteCount(list.SiteCount) {}

std::pair< size_t, size_t > SegmentList::Find(size_t index)
{
    size_t left = 0;
    size_t node = Root;
    while ( left + Pool->GetSize(node) <= index)
    {
        left += Pool->GetSize(node);
        node = Pool->GetNext(node);
    }

    return {node, left};
}

Byte* SegmentList::GetData(size_t index)
{
    auto found = Find(index);
    return Pool->GetData(found.first, index-found.second);
}

void SegmentList::Overwrite(size_t index, const std::vector<std::byte>& segment)
{
    auto found = Find(index);
    auto left = found.first;
    auto leftOffset = index-found.second;

    size_t startIndex = 0;

    while(startIndex < segment.size())
    {
        // overwrite the node if current genome is only one with data
        if (Pool->Unique(left))
        {
            startIndex = Pool->Overwrite(left, leftOffset, segment, startIndex);
            leftOffset = 0;
        }

        // create a new node to replace
        else
        {      
            size_t right = left;
            size_t rightOffset = leftOffset;

            size_t secondIndex = startIndex;

            while(!Pool->Unique(right) && secondIndex < segment.size())
            {
                // if what's left of the current node is less than rest of segment
                if (secondIndex + Pool->GetSize(right) - rightOffset < segment.size())
                {
                    secondIndex += Pool->GetSize(right) - rightOffset;
                    right = Pool->GetNext(right);
                    rightOffset = 0;
                }
                // end of data is in node
                else
                {
                    rightOffset = segment.size() - secondIndex;
                    secondIndex = segment.size();
                }
            }


            // create new mutation
            size_t mutation = Pool->Allocate(std::make_shared< std::vector<Byte> >(segment.begin()+startIndex, segment.begin()+secondIndex));
            
            // edit left and right node
            if (leftOffset)
                Pool->TruncateRight(left, leftOffset);
            else
            {
                left = Pool->GetPrev(left);
                if (left)
                    Pool->SetNext(left, mutation);
                else
                    Root = mutation;
            }  

            if (rightOffset == Pool->GetSize(right))
                right = Pool->GetNext(right);
            else
                Pool->TruncateLeft(right, rightOffset);

            Pool->SetNext(mutation, right);

            startIndex = secondIndex;
            left = right;

        }

        left = Pool->GetNext(left);
    }
}

void SegmentList::Insert(size_t index, const std::vector<std::byte>& segment)
{
    auto found = Find(index);
    auto left = found.first;
    auto leftOffset = index-found.second;

    if (Pool->Unique(left))
    {
        Pool->Insert(left, leftOffset, segment);
    }
    else
    {
        // create new mutation
        size_t mutation = Pool->Allocate(std::make_shared< std::vector<Byte> >(segment));
        size_t right = left;
        if (leftOffset == Pool->GetSize(left))
        {
            right = Pool->GetNext(right);
        }
        else
        {
            right = Pool->Copy(left);
            Pool->TruncateRight(left, leftOffset+1);
            Pool->TruncateLeft(right, leftOffset+1); 
        }
        
        Pool->SetNext(left, mutation);
        Pool->SetNext(mutation, right);
    }

}

void SegmentList::Remove(size_t index, size_t segmentSize)
{
    auto found = Find(index);
    auto left = found.first;
    auto offset = index-found.second;

    auto right = left;

    while(segmentSize)
    {
        // remove all
        if (!offset && Pool->GetSize(right) <= segmentSize)
        {
            if (left == right)
                left = Pool->GetPrev(left);

            segmentSize -= Pool->GetSize(right);
            Pool->Clear(right);
            right = Pool->GetNext(right);
            offset = 0;
        }

        // remove part 
        else 
        {
            if (Pool->Unique(right)) // IF UNIQUE
            {
                Pool->Remove(right, offset, segmentSize);
            }

            else
            {
                if (left == right)
                {
                    if (offset)
                    {
                        right = Pool->Copy(left);
                        Pool->TruncateRight(left, offset);
                    }
                    else
                        left = Pool->GetPrev(left);
                    
                }

                Pool->TruncateLeft(right, offset+segmentSize);
            }
            segmentSize = 0;   
        }
    }

    if (left != right)
    {
        Pool->SetNext(left, right);
    }

    if (!left)
        Root = right;
}

void SegmentList::Print()
{
    size_t node = Root;
    while (node)
    {
        Pool->Print(node);
        node = Pool->GetNext(node);
        std::cout << node << std::endl;
    }
}
