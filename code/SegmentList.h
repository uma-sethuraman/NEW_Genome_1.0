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
#include "IndexTable.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a Segment List object **/
class SegmentList
{
private:
    /// Gives handler access to variables
    friend class Handler; 
    friend class MutatorHandler;
    friend class IndexTable;

    /// Data variables
    size_t Size = 1; ///< number of nodes in list
    size_t SiteCount;   ///< sum of sites from all nodes

    /// Linked list variables
    SegmentNode* Head;

    /// fast access
    IndexTable* Table;

public:
    /** (deleted) Default constructor **/
    SegmentList() = delete;

    /** Constructor 
     * \param genome to construct from **/
    SegmentList(std::shared_ptr< GeneSegment > genome) 
        :  SiteCount(genome->size()), Head(new SegmentNode(genome)) 
    {
        Table = new IndexTable(Head);
    }

    /** Deconstructor **/
    ~SegmentList() 
    { 
        delete Head; 
        delete Table;
    }

    /** gets number of segments in list
     * \return size **/
    const size_t size() { return Size; }
    /** gets total number of sites from all segments
     * \return siteCount **/
    const size_t siteCount() { return SiteCount; }

    std::pair<size_t, SegmentNode*> Search(size_t index)
    {
        return Table->Search(index);
    }

   /** Prints Segment node **/
    void print()
    {
        SegmentNode* curr = Head;
        while (curr != nullptr)
        {
            curr->print();
            curr = curr->next();
        }
    }
    
};