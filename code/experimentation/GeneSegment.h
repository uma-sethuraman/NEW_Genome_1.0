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

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class GeneSegment
{
private:
    Byte* Gene; ///< new byte of data
    const size_t Size;

    bool NewData;

public:
    /** (deleted) Default Constructor **/
    GeneSegment() = delete;

    /** Constructor 
     * \param value value to put into gene **/
    GeneSegment(Byte* gene, size_t size, bool newData = false) 
        : Gene(gene), Size(size), NewData(newData)
    {
        if (NewData)
        {
            Gene = new Byte[Size];
            std::memcpy(Gene, gene, Size);
        }
        else
        {
            Gene = gene;
        }
    }

    /** Deconstructor **/
    ~GeneSegment() 
    { 
        if (NewData)
            delete[] Gene; 
    }

    /** Gets data of gene 
     * \return pointer to data **/
    Byte* data() { return Gene; }
    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    size_t size() { return Size; }


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