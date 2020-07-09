/**
 * \file SegmentTree.h
 * \author Victoria Cao
 * 
 * \brief SegmentTree class that has an array of bytes containing
 * physical values of a SegmentTree
 **/

#pragma once

#define leafSize 6

#include <memory>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "GeneSegment.h"


struct Key
{
    std::shared_ptr< GeneSegment > Gene = nullptr;
    size_t Size;

    Key() = default;
    Key(size_t size, std::shared_ptr< GeneSegment > gene) : Size(size), Gene(gene) {}

};

class SegmentNode
{
private:
    friend class SegmentTree;

    Key Keys[leafSize - 1];
    size_t Child[leafSize];

    size_t Size;    ///< number of keys in node
    size_t Weight;  ///< 
    size_t Sum;
    size_t Pos;

    bool Leaf = true;

public:
    SegmentNode(size_t pos) : Pos(pos) {}
};
