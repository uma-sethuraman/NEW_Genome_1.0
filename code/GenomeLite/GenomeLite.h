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
#include "SegmentList.h"


typedef std::byte Byte;


/** Implementation of a genomeLite object **/
class GenomeLite : public AbstractGenome
{
private:
    SegmentList* List;

    void Reallocate();

public:
    /** (deleted) default constructor **/
    GenomeLite() = delete;

    /** size constructor **/
    GenomeLite(size_t size) 
        : AbstractGenome(size), List(new SegmentList(size)) {}

    /** size constructor **/
    GenomeLite(const GenomeLite &genome) 
        : AbstractGenome(genome.size_), List(new SegmentList(*(genome.List))) {}

    /** destructor **/
    ~GenomeLite() { delete List; }

    /** Gets size of genome sites
     * \return size of tree **/
    virtual size_t size() { return List->GetSiteCount(); }

    virtual AbstractGenome* clone() override;
    virtual Byte* data(size_t index = 0, size_t byteSize = 0) override;
    virtual void overwrite(size_t index, const std::vector<std::byte>& segment) override;
    virtual void insert(size_t index, const std::vector<std::byte>& segment) override;
    virtual void remove(size_t index, size_t segmentSize) override;
    virtual void show() override;
    virtual std::vector<size_t> find_all(std::vector<std::byte>& pattern) override;

};