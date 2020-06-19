/**
 * \file Mutation.h
 * \author Victoria Cao
 * 
 * \brief Segment node class for segmentation of data
 * implementation
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class Mutation
{
private:
    Byte* Gene; ///< new byte of data
    size_t Size;

public:
    /** (deleted) Default Constructor **/
    Mutation() = delete;

    /** Constructor 
     * \param value value to put into gene **/
    template < typename T >
    Mutation(T value)
    {
        Gene = reinterpret_cast<Byte*>(&value);
    }

    /** Deconstructor **/
    ~Mutation() { }

    /** Gets data of gene 
     * \return pointer to data **/
    Byte* data() { return Gene; }
    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    size_t size() { return Size; }



};