/**
 * \file GenomeLite.cpp
 * \author Victoria Cao
 **/

#include "GenomeLite.h"

/** Reallocates the tree
 **/
void GenomeLite::Reallocate()
{

}

/** Gets contiguous data from genome
 * \param index to start
 * \param byteSize number of bytes
 * \returns start of memory
 **/
Byte* GenomeLite::data(size_t index, size_t byteSize)
{
    return Tree->GetData(index, byteSize);
}

/** Overwrites an index in the gene
 * \param index
 * \param segment
 **/
void GenomeLite::overwrite(size_t index, std::vector<std::byte> segment)
{
    Tree->Overwrite(index, segment);
}

/** Inserts at index in the gene
 * \param index
 * \param segment
 **/
void GenomeLite::insert(size_t index, std::vector<std::byte> segment)
{

}

/** Removes at index in the gene
 * \param index
 * \param segmentSize
 **/
void GenomeLite::remove(size_t index, size_t segmentSize)
{

}

/** Prints the tree **/
void GenomeLite::show()
{
    Tree->Print();
}
