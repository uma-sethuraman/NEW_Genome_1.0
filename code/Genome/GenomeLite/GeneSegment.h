/**
 * \file GeneSegment.h
 * \author Victoria Cao
 * 
 * \brief GeneSegment class that has an array of bytes containing
 * physical values of a GeneSegment
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

typedef std::byte Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class GeneSegment
{
private:
    friend class SegmentNode; 
    friend class SegmentTree;

    Byte* Gene; ///< new byte of data
    const size_t MaxSize;
    size_t Size;


public:

    /** (deleted) Default Constructor **/
    GeneSegment() = delete;

    /** Constructor 
     * \param size of genome **/
    GeneSegment(size_t maxSize, size_t size = 0)
        : Gene(new Byte[maxSize]), MaxSize(maxSize), Size(size) {};


    /** Deconstructor **/
    ~GeneSegment() 
    { 
        delete[] Gene; 
    }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    bool IsFull() { return Size == MaxSize; }


   /** Prints linked list segment nodes**/
    void Print()
    {
        std::cout << "size: " << Size << " Values: ";
        for (size_t i = 0; i < Size; i++)
        {
            std::cout << (int)Gene[i] << ", ";
        }
        std::cout << std::endl;
    }

};