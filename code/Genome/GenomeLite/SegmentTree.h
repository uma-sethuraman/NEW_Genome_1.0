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

#include "MemoryPool.h"
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
    MemoryPool* Pool; ///< allocation pool for nodes


    /// helper functions
    SegmentNode* Initialize(size_t size);

    int GetHeight(SegmentNode* node);
    int GetBalance(SegmentNode* node);
    void UpdateHeight(SegmentNode* node);
    void UpdateWeight(SegmentNode* node);
    void Update(SegmentNode* node);

    ///.rebalancing functions
    SegmentNode* RotateRight(SegmentNode* node);
    SegmentNode* RotateLeft(SegmentNode* node);
    SegmentNode* ReBalance(SegmentNode* node);


public:
    /** (deleted) Default constructor **/
    SegmentTree() = delete;

    /** Constructor 
     * \param genome new root of tree **/
    SegmentTree(size_t size);

    /** Copy constructor
     * \param tree **/
    SegmentTree(const SegmentTree &tree)
        : Size(tree.Size), Pool(new MemoryPool(*(tree.Pool))) 
    {
        size_t index = tree.Root->GetPos();
        Root = Pool->At(index);
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

    Byte* GetData(size_t index = 0, size_t byteSize = 0);

    /// mutation functions
    std::pair<SegmentNode*, size_t> Find(size_t index);
    void Delete(size_t index, size_t segmentSize);
    void Insert(size_t index, std::vector<Byte > segment);
    void Overwrite(size_t index, std::vector<Byte > segment);
    void Print();

};