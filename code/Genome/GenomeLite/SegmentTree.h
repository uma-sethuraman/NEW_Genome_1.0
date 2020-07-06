/**
 * \file SegmentTree.h
 * \author Victoria Cao
 * 
 * \brief class for a B+ based tree made up of segment nodes
 **/

#pragma once
#include <cstdint>

#include "SegmentNode.h"

/** class for a memory pool object **/
class SegmentTree
{
private:
    /// memory allocation
    MemoryPool* Pool;

    size_t Size;
    size_t SiteCount;

    void SplitChild(SegmentNode* node, size_t index);

public:
    /** (deleted) default constructor **/
    SegmentTree() = delete;

    /** Constructor 
     * \param size **/
    SegmentTree(size_t size) 
        : Pool(size*(3/4)) {}

    /** destructor **/
    ~SegmentTree() { delete Pool; }

    /** Gets number gene references in tree
     * \return size **/
    const size_t GetSize() { return Size; }

    /** Gets sites in genome
     * \return sitecount **/
    const size_t GetSiteCount() { return SiteCount; }

    SegmentNode* Find(size_t index);
    void Insert(GeneSegment &segment, size_t index);

};