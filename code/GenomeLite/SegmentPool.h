/**
 * \file SegmentPool.h
 * \author Victoria Cao
 * 
 * \brief SegmentPool to localize all nodes for a SegmentTree
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>
#include <deque>
#include <algorithm>

#include"SegmentNode.h"

typedef std::byte Byte; // c++17 Byte doesn't always work
typedef std::vector< std::byte > Gene;

/** Implementation of a genom object **/
class SegmentPool
{
private:
    std::vector< SegmentNode > Pool; //< Pool of segmentNodes

public:
    /** (deleted) Default Constructor **/
    SegmentPool() = delete;

    /** Constructor 
     * \param size value to put into gene **/
    SegmentPool(size_t size) 
    {
        Pool.reserve(size);
        Pool.push_back(SegmentNode());
    }

    /** Copy constructor
     * \param pool **/
    SegmentPool(const SegmentPool &pool)
        : Pool(pool.Pool) {}

    /** Deconstructor **/
    ~SegmentPool() { Pool.clear(); }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t GetSize() { return Pool.capacity(); }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t GetTail() { return Pool.size(); }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const bool IsFull() { return Pool.size() >= Pool.capacity()-10; }


    /** Creates a new node in the pool
     * \param data to make SegmentNOde from
     * \return pointer to newly allocated node **/
    size_t Allocate(std::shared_ptr< Gene > data)
    {
        Pool.push_back(SegmentNode(data, Pool.size())); // assign node the data
        return Pool.size()-1;
    }

    /** Creates a new node in the pool
     * \param index of node to copy
     * \return index of newly copied node **/
    size_t Copy(size_t index) 
    {
        Pool.push_back(SegmentNode(Pool[index], Pool.size())); // assign node the data
        return Pool.size()-1;
    }

    /** Creates a new node in the pool
     * \param index of node to copy
     * \return index of newly copied node **/
    size_t Split(size_t index) 
    {
        size_t cutIndex = Pool.size();
        auto cutSize = Pool[index].Size/2;

        Pool.push_back(SegmentNode(std::make_shared< Gene >(Pool[index].Data->begin()+cutSize, Pool[index].Data->end()), Pool.size()));
      
        Pool[cutIndex].Next = Pool[index].Next;
        Pool[index].Next = cutIndex;

        Pool[index].Size -= cutSize;
        Pool[index].Data->resize(Pool[index].Size);

        return cutIndex;
    }

    /** Gets use count 
     * \returns use count of the smart pointer **/
    bool Unique(size_t index) 
    { 
        return Pool[index].Data.use_count() == 1; 
    }

    /** Gets use count 
     * \returns use count of the smart pointer **/
    bool Allocated(size_t index) 
    { 
        return Pool[index].Data != nullptr; 
    }

    /** Gets data from Segment
     * \returns size of data **/
    Byte* GetData(size_t index, size_t localIndex)
    {
        return &(Pool[index].Data->at(localIndex));
    }

    const size_t GetPos(size_t index) 
    {   
        return Pool[index].Pos; 
    }

    const size_t GetSize(size_t index) 
    {   
        return Pool[index].Size; 
    }

    long GetPrev(size_t index)
    {
        return Pool[index].Prev;
    }
    
    long GetNext(size_t index)
    {
        return Pool[index].Next;
    }
    
    void SetPrev(size_t index, size_t prev)
    {
        if (prev)
        {
            Pool[index].Prev = prev;
            Pool[prev].Next = index;
        }
        else
        {
            Pool[index].Prev = 0;
        }
    }

    void SetNext(size_t index, size_t next)
    {
        if (next)
        {
            Pool[index].Next = next;
            Pool[next].Prev = index;
        }
        else
        {
            Pool[index].Next = 0;
        }
    }

    void TruncateLeft(size_t index, size_t localIndex)
    {
        Pool[index].Start += localIndex;
        Pool[index].Size -= localIndex;
    }

    void TruncateRight(size_t index, size_t localIndex)
    {
        Pool[index].Size = localIndex;
    }

    size_t Overwrite(size_t index, size_t localIndex, const Gene& segment, size_t start)
    {
        auto size = Pool[index].Size-localIndex;
        std::move(segment.begin(), segment.end(), Pool[index].Data->begin()+localIndex);

        return start+size;
    }

    void Insert(size_t index, size_t localIndex, const Gene& segment)
    {
        auto newSize = Pool[index].Data->size()+segment.size();
        Pool[index].Size = newSize;

        auto newData = std::make_shared< Gene >(newSize);

        std::copy_n(Pool[index].Data->begin(), localIndex, newData->begin());
        std::copy_n(segment.begin(), segment.size(), newData->begin()+localIndex);
        std::copy_n(Pool[index].Data->begin()+localIndex, Pool[index].Data->size()-localIndex, newData->begin()+localIndex+segment.size());

        Pool[index].Data.swap(newData);

    }

    void Remove(size_t index, size_t localIndex, size_t segmentSize)
    {
        Pool[index].Size -= segmentSize;
        auto data = Pool[index].Data;
        
        data->erase(data->begin()+localIndex, data->begin()+localIndex+segmentSize);
    }

    void Clear(size_t index)
    {
        Pool[index].Data.reset();
    }

    /** prints node info **/
    void Print(size_t index)
    {
        SegmentNode node = Pool[index];
        std::cout << "SegmentNode " << &Pool[index] << "\tsize " << node.Size << "\tpos " << node.Pos << std::endl;
        std::cout << "B " << (unsigned long)node.Start << std::endl;
        std::cout << "E " << (unsigned long)node.Start+node.Size << std::endl;
        
        for (size_t i = 0; i < node.Size; i++)
        {
            std::cout << std::dec << (int)node.Data->at(node.Start+i) << ", ";
        }
        std::cout << std::endl;
        std::cout << "prev " << node.Prev << std::endl;
        std::cout << "next " << node.Next << std::endl;
        std::cout << std::endl;
    }


    void Print()
    {
        for (size_t i(0); i < Pool.size(); ++i)
        {
            Print(i);
        }
    }

};