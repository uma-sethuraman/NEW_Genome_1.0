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
#include "SegmentNode.h"

typedef char Byte; // easy reading

/** basic implementation of rope **/
class SegmentTree
{
private:
    SegmentNode* Root; ///< Root node of tree

    size_t Size = 1;


    /// helper functions
    int GetHeight(SegmentNode* node);
    int GetBalance(SegmentNode* node);

    SegmentNode* RotateRight(SegmentNode* node);
    SegmentNode* RotateLeft(SegmentNode* node);
    void UpdateHeight(SegmentNode* node);
    void UpdateWeight(SegmentNode* node);

    SegmentNode* ReBalance(SegmentNode* node);
    void Update(SegmentNode* node);

    void DeleteTree(SegmentNode* node);

public:
    /** Default constructor **/
    SegmentTree() = delete;

    /** Default constructor **/
    SegmentTree(SegmentNode* root) 
        : Root(root) {}

    /** Destructor **/
    ~SegmentTree() { DeleteTree(Root); }

    std::pair<SegmentNode*, size_t> Find(size_t index);

    void Delete(size_t index);
    void Insert(SegmentNode* root, size_t index);
    void print();

};