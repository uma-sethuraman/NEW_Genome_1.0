/**
 * \file SegmentTree.h
 * \author Victoria Cao
 * 
 * \brief class for a rope data structure
 **/

#pragma once

#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <queue>
#include <cmath>

#include "GeneSegment.h"
#include "SegmentPool.h"
#include "SegmentNode.h"

typedef std::byte Byte; // easy reading

/** basic implementation of rope **/
class SegmentTree
{
private:
    friend class GenomeLite;

    /// member variables
    SegmentNode* Root; ///< Root node of tree
    size_t Size = 1;    ///< Size of tree
    SegmentPool* Pool; ///< allocation pool for nodes


public:
    

};