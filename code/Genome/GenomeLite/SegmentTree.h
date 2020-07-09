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


    /// helper functions
    int GetHeight(SegmentNode* node);
    int GetBalance(SegmentNode* node);
    void UpdateHeight(SegmentNode* node);
    void UpdateWeight(SegmentNode* node);
    void Update(SegmentNode* node);
    SegmentNode* CreateNode(size_t size);

    ///.rebalancing functions
    SegmentNode* RotateRight(SegmentNode* node);
    SegmentNode* RotateLeft(SegmentNode* node);
    SegmentNode* ReBalance(SegmentNode* node);

    /// helper function
    void DeleteTree(SegmentNode* node);
    SegmentNode* Initialize(size_t size);

public:
    /** (deleted) Default constructor **/
    SegmentTree() = delete;

    /** Constructor 
     * \param genome new root of tree **/
    SegmentTree(size_t size) 
        : Pool(new SegmentPool(size*3/4))
    {
        Root = Initialize(size);
    }

    /** Copy constructor
     * \param tree **/
    SegmentTree(const SegmentTree &tree)
        : Size(tree.Size), Pool(new SegmentPool(*(tree.Pool))) 
    {
        size_t index = tree.Root->GetPos();
        Root = Pool->GetNode(index);
    }

    /** Destructor **/
    ~SegmentTree() { delete Pool; }

    /** Gets root
     * \return root **/
    SegmentNode* GetRoot() { return Root; };

    /** Gets size
     * \return size **/
    const size_t GetSize() { return Size; };

    /** Gets number of sites
     * \return size **/
    const size_t GetSiteCount() { return Root->GetWeight(); };

    Byte* GetData(size_t index);

    /// mutation functions
    std::pair<SegmentNode*, size_t> Find(size_t index);
    void Delete(size_t index, size_t segmentSize);
    void Insert(size_t index, std::vector<std::byte> &segment);
    void Overwrite(size_t index, std::vector<std::byte> &segment);
    void Print();

};