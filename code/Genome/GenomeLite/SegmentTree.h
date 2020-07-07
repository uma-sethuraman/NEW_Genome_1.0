/**
 * \file SegmentTree.h
 * \author Victoria Cao
 * 
 * \brief class for a B+ based tree made up of segment nodes
 **/

#pragma once
#include <cstdint>

#include "SegmentNode.h"
#include "MemoryPool.h"

/** class for a memory pool object **/
class SegmentTree
{
private:
    /// memory allocation
    MemoryPool< SegmentNode >* NodePool;
    SegmentNode* Root;

    size_t Size = 0;
    size_t SiteCount = 0;

    void SplitChild(SegmentNode* node, size_t index);
    std::pair<SegmentNode*, size_t> Find(size_t index);

public:
    /** (deleted) default constructor **/
    SegmentTree() = delete;

    /** Constructor 
     * \param size **/
    SegmentTree(size_t size) 
        : NodePool(new MemoryPool< SegmentNode >(size*(3/4))) {}

    /** destructor **/
    ~SegmentTree() 
    { 
        delete NodePool;   
    }

    /** Gets number gene references in tree
     * \return size **/
    const size_t GetSize() { return Size; }

    /** Gets sites in genome
     * \return sitecount **/
    const size_t GetSiteCount() { return SiteCount; }

    void Insert(GeneSegment &segment, size_t index);

};