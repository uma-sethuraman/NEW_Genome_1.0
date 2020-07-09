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
    
    Byte* Gene; ///< new byte of data
    const size_t MaxSize;
    size_t Size;
    

public:

    /** (deleted) Default Constructor **/
    GeneSegment() = delete;

    /** Constructor 
     * \param size of the gene segment **/
    GeneSegment(size_t size)
        : Gene(new Byte[size]), MaxSize(size), Size(size) {}


    GeneSegment(size_t maxSize, size_t size)
        : Gene(new Byte[maxSize]), MaxSize(maxSize), Size(size) {}

    GeneSegment(const std::vector< Byte > &segment, size_t size, size_t maxSize, size_t startSegment = 0)
        : Gene(new Byte[maxSize]), MaxSize(maxSize), Size(size) 
    {
        std::memcpy(Gene, &segment[startSegment], Size);
    }


    /** Deconstructor **/
    ~GeneSegment() 
    { 
        delete[] Gene; 
    }

    /** Gets data of gene 
     * \return pointer to data **/
    Byte* GetData() { return Gene; }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    size_t GetSize() { return Size; }


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