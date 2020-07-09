/**
 * \file GenomeLite.h
 * \author Victoria Cao
 * 
 * \brief GenomeLite class that has an array of bytes containing
 * physical values of a GenomeLite
 **/

#pragma once
#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "GeneSegment.h"
#include "SegmentNode.h"
#include "SegmentPool.h"

typedef std::byte Byte; // c++17 std::byte doesn't always work



/** Implementation of a genom object **/
class GenomeLite
{
public:
    SegmentPool* Pool;
    SegmentNode* Root;

    size_t Size;
    size_t SiteCount;


    GenomeLite(size_t size);

    std::pair<SegmentNode*, size_t> Find(size_t index);


    void SplitChild(SegmentNode* node);


    void overwrite(size_t index, const std::vector< Byte > &segment);


};