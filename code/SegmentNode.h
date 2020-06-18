/**
 * \file Genome.h
 * \author Victoria Cao
 * 
 * \brief Base class for a simple genome class using SegmentNode 
 * implementation
 **/

#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>
#include "AbstractGenome.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class Genome : public AbstractGenome {
private:
	const Byte* start; ///< start of memory
    Byte* end;  ///< end of memory
    int size;

public:
    /** (deleted) Default Constructor */
    SegmentNode() = delete;

    /** Constructor
     * \param start
     * \param end */
    SegmentNode(Byte* startAddr, Byte* endAddr, int newSize) 
        : start(startAddr), end(endAddr), size(newSize)
    {}

    ~SegmentNode() {}
};
