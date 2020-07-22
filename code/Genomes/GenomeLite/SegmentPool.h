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
    {
        Pool.reserve(pool.Pool.capacity());
        Pool.resize(pool.Pool.size());
        std::copy_n(pool.Pool.begin(), pool.Pool.size(), Pool.begin());
    }

    /** Deconstructor **/
    ~SegmentPool() { Pool.clear(); }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t Capacity() { return Pool.capacity(); }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t Size() { return Pool.size(); }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const bool IsFull() { return Pool.size()+4 > Pool.capacity(); }


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
        Pool[Pool.back().Next].Prev = Pool.size()-1;
        
        return Pool.size()-1;
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
        return &(Pool[index].Data->at(Pool[index].Start+localIndex));
    }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    const size_t GetCapacity(size_t index) 
    {  
        return Pool[index].Data->capacity(); 
    }

    /** Gets position
     * \param index to get position **/
    const size_t GetPos(size_t index) 
    {   
        return Pool[index].Pos; 
    }

    /** Gets size
     * \param index to get size **/
    const size_t GetSize(size_t index) 
    {   
        return Pool[index].Size; 
    }

    /** Gets previous
     * \param index to get previous **/
    long GetPrev(size_t index)
    {
        return Pool[index].Prev;
    }
    
    /** Gets next
     * \param index to get next **/
    long GetNext(size_t index)
    {
        return Pool[index].Next;
    }
    
    /** Sets previous
     * \param index to get previous 
     * \param prev to set it to **/
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

    /** Sets next
     * \param index to get next 
     * \param next to set it to **/
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

    /** Truncates the node data from left to local index
     * \param index to get node
     * \param localIndex to truncate to **/
    void TruncateLeft(size_t index, size_t localIndex)
    {
        Pool[index].Start += localIndex;
        Pool[index].Size -= localIndex;
    }

    /** Truncates the node data from right to local index
     * \param index to get node
     * \param localIndex to truncate to **/
    void TruncateRight(size_t index, size_t localIndex)
    {
        Pool[index].Size = localIndex;
    }

    /** Overwrites the data on node
     * \param index to get node
     * \param localIndex to start overwriting
     * \param segment to use for overwriting
     * \param start index into segment to start copying
     * \param size to overwrite **/
    void Overwrite(size_t index, size_t localIndex, const Gene& segment, size_t start, size_t size)
    {
        std::copy_n(segment.begin()+start, size, Pool[index].Data->begin()+localIndex);
    }

    /** Overwrites the data on node
     * \param index to get node
     * \param localIndex to start overwriting
     * \param data to overwrite with
     * \param size to overwrite **/
    void Overwrite(size_t index, size_t localIndex, Byte* data, size_t size)
    {
        std::copy_n(data, size, Pool[index].Data->begin()+localIndex);
    }

    /** Inserts the data into node
     * \param index to get node
     * \param localIndex to start inserting
     * \param segment to insert **/
    void Insert(size_t index, size_t localIndex, const Gene& segment)
    {
        Pool[index].Size += segment.size();

        Pool[index].Data->insert(Pool[index].Data->begin()+localIndex, segment.begin(), segment.end());
    }

    /** Removes the data into node
     * \param index to get node
     * \param localIndex to start removing
     * \param segmentSize to remove **/
    void Remove(size_t index, size_t localIndex, size_t segmentSize)
    {
        Pool[index].Size -= segmentSize;
        auto data = Pool[index].Data;
        
        data->erase(data->begin()+localIndex, data->begin()+localIndex+segmentSize);
    }

    /** Resizes the nodedata
     * \param index to get node
     * \param newSize of data **/
    void Resize(size_t index, size_t newSize)
    {
        Pool[index].Data->resize(newSize);
    }

    /** Clears the smart pointer at index
     * \param index to get node **/
    void Clear(size_t index)
    {
        Pool[index].Data.reset();
    }

    /** prints node info 
     * \param index to get node **/
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


    /** Prints the Pool **/
    void Print()
    {
        for (size_t i(0); i < Pool.size(); ++i)
        {
            Print(i);
        }
    }

};