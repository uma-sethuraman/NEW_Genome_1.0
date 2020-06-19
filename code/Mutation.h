/**
 * \file Mutation.h
 * \author Victoria Cao
 * 
 * \brief Mutation class that has an array of bytes containing
 * physical values of a mutation
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class Mutation
{
private:
    Byte* Gene; ///< new byte of data
    const size_t Size;

public:
    /** (deleted) Default Constructor **/
    Mutation() = delete;

    /** Constructor 
     * \param value value to put into gene **/
    template < typename T >
    Mutation(T value) : Size(sizeof(T))
    {
        Gene = new Byte[Size];
        std::memcpy(Gene, &value, Size*sizeof(Byte));
    }

    /** Deconstructor **/
    ~Mutation() { delete[] Gene; }

    /** Gets data of gene 
     * \return pointer to data **/
    Byte* data() { return Gene; }
    /** Gets number of Bytes of data
     * \return number of bytes of data **/
    size_t size() { return Size; }

};