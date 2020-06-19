/**
 * \file SegmentList.h
 * \author Victoria Cao
 * 
 * \brief Class for keeping track of a linked list of 
 * Segment NOdes
 **/

#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <iostream>

#include "AbstractGenome.h"
#include "SegmentNode.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a Segment List object **/
class SegmentList
{
private:
    /// Gives handler access to variables
    friend class Handler; 
    friend class MutatorHandler;

    /// Data variables
    size_t Size = 1; ///< number of nodes in list
    size_t SiteCount;   ///< sum of sites from all nodes

    /// Linked list variables
    std::shared_ptr<SegmentNode> StartSegment;

public:
    /** (deleted) Default constructor **/
    SegmentList() = delete;

    /** Constructor 
     * \param genome to construct from **/
    SegmentList(std::shared_ptr< GeneSegment > genome) 
        :  SiteCount(genome->size()), StartSegment(std::make_shared<SegmentNode>(genome)) {}

    /** gets number of segments in list
     * \return size **/
    const size_t size() { return Size; }
    /** gets total number of sites from all segments
     * \return siteCount **/
    const size_t siteCount() { return SiteCount; }
    
};