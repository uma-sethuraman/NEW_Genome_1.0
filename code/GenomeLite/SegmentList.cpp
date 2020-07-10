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

    SegmentNode* node = Root;
    SegmentNode* newNode;
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
        node->SetNext(newNode);
        node = newNode;
    }
}

SegmentList::SegmentList(const SegmentList &list)
    : Pool(new SegmentPool(*(list.Pool))), Size(list.Size), SiteCount(list.SiteCount)
{
    Root = Pool->At(list.Root->GetPos());
}


std::pair< SegmentNode*, size_t > SegmentList::Find(size_t index)
{
    size_t left = 0;
    SegmentNode* node = Root;
    while ( left + node->Size <= index)
    {
        left += node->Size;
        node = node->GetNext(Pool);
    }

    return {node, left};
}

Byte* SegmentList::GetData(size_t index)
{
    auto found = Find(index);
    return found.first->GetData(index-found.second);
}

std::pair< SegmentNode*, size_t > SegmentList::FindEnd(SegmentNode* node, size_t start, size_t size)
{
    auto offset = start;
    while (size)
    {
        if (size > node->Size-offset)
        {
            size -= node->Size-offset;
        }
        else
        {
            offset = size;
            size = 0;
        }
        
        offset = 0;
        node = node->GetNext(Pool);
    }

    return {node, offset};
}

void SegmentList::Overwrite(size_t index, const std::vector<std::byte>& segment)
{
    auto found = Find(index);
    auto node = found.first;
    auto offset = found.second;

    size_t startIndex = 0;

    while(startIndex < segment.size())
    {
        if (node->Unique())
        {
            startIndex = node->Overwrite(offset, segment, startIndex);
            offset = 0;
        }
        else
        {   
            SegmentNode* left = node;    
            found = FindEnd(node, offset, segment.size()-startIndex);
            auto right = found.first;
            auto rightOffset = found.second;

        }

        node = node->GetNext(Pool);
    }
}

void SegmentList::Insert(size_t index, const std::vector<std::byte>& segment)
{

}

void SegmentList::Remove(size_t index, size_t segmentSize)
{

}

void SegmentList::Print()
{
    SegmentNode* node = Root;
    while (node)
    {
        node->Print();
        node = node->GetNext(Pool);
    }
}
