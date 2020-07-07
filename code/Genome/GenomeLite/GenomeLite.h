/**
 * \file GenomeLite.h
 * \author Victoria Cao
 * 
 * \brief Base class for a simple GenomeLite class using SegmentNode 
 * implementation
 **/

#pragma once
#include <cstdint>
#include <iostream>

#include "../AbstractGenome.h"
#include "SegmentTree.h"


typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genomeLite object **/
class GenomeLite : public AbstractGenome
{
private:
    SegmentTree* Tree;

    void Reallocate();

public:
    /** (deleted) default constructor **/
    GenomeLite() = delete;

    /** size constructor **/
    GenomeLite(size_t size) 
        : AbstractGenome(size), Tree(new SegmentTree(size)) {}

    /** destructor **/
    ~GenomeLite() { delete Tree; }

    /** Gets size of genome sites
     * \return size of tree **/
    virtual size_t size() { return Tree->GetSiteCount(); }

    virtual void initalize() override;
    virtual void overwrite(size_t index, std::vector<std::byte> segment) override;
    virtual void insert(size_t index, std::vector<std::byte> segment) override;
    virtual void remove(size_t index, size_t segmentSize) override;


};