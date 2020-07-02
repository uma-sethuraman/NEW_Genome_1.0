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

typedef char Byte; // easy reading

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
    SegmentNode* CreateNode(Byte* data, size_t size);

    ///.rebalancing functions
    SegmentNode* RotateRight(SegmentNode* node);
    SegmentNode* RotateLeft(SegmentNode* node);
    SegmentNode* ReBalance(SegmentNode* node);

    /// helper function
    void DeleteTree(SegmentNode* node);


public:
    /** (deleted) Default constructor **/
    SegmentTree() = delete;

    /** Constructor 
     * \param genome new root of tree **/
    SegmentTree(std::shared_ptr< GeneSegment > gene) 
    {
        Pool = new SegmentPool((size_t)pow(gene->GetSize(), 0.75));
        Root = Pool->CreateNode(gene);
    }

    /** Constructor 
     * \param genome new root of tree **/
    SegmentTree(Byte* data, size_t size, bool newData = false) 
    {
        Pool = new SegmentPool(size/2);
        Root = Pool->CreateNode(std::make_shared< GeneSegment >(data, size, newData));
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

    /// mutation functions
    std::pair<SegmentNode*, size_t> Find(size_t index);
    void Delete(size_t index);
    void Insert(size_t index, Byte* mutation, size_t size);
    void Point(size_t index, Byte* mutation, size_t size);
    void Print();

};