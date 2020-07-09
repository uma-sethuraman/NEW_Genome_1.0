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
#include <vector>

typedef std::byte Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class GeneSegment
{
private:
    friend class SegmentNode; 
    
    std::vector< Byte > Gene;
    

public:

    /** Deconstructor **/
    ~GeneSegment() 
    { 
        delete[] Gene; 
    }

    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    size_t GetSize() { return Size; }


   /** Prints linked list segment nodes**/
    void Print()
    {
        std::cout << "size: " << Size << " Values: ";
        for (size_t i = 0; i < Gene.size(); i++)
        {
            std::cout << (int)Gene[i] << ", ";
        }
        std::cout << std::endl;
    }

};