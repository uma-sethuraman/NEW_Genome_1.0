/**
 * \file IndexTable.h
 * \author Victoria Cao
 * 
 * \brief Class for fast indexing into SegmentList
 **/

#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>
#include <iostream>
#include <map>
#include <utility>
#include <algorithm>

#include "SegmentNode.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a Segment List object **/
class IndexTable
{
private:
    /// enable Segment list access
    friend class SegmentList;

    /// Data variables
    std::map<size_t, std::map< size_t, SegmentNode* >*> Forest;


public:
    /** (deleted) Default constructor **/
    IndexTable();

    /** Constructor 
     * \param node First segment index 0 **/
    IndexTable(SegmentNode* node)
    {
        std::map< size_t, SegmentNode* >* newBranch = new std::map< size_t, SegmentNode* >({ {0, node} });
        std::pair< size_t, std::map< size_t, SegmentNode* >* > newTree = {0, newBranch};
        Forest.insert( newTree );
    }

    /** Search tree for index
     * \param index to search for
     * \return starting index of the segment the index is within starts at
     * and pointer to the segment **/
    std::pair<size_t, SegmentNode*> Search(size_t index)
    {
        size_t branchKey = (index && 0xFFFFFF00) >> 8;
        size_t indexKey = index & 0xFF;

        // find closest lower bounded branch
        auto rangeBranch = Forest.lower_bound(branchKey);
        if(rangeBranch != Forest.begin() && rangeBranch->first != branchKey) {
            rangeBranch--;
        }

        // find closest node on tree
        auto indexPair = rangeBranch->second->lower_bound(indexKey);
        if(indexPair != rangeBranch->second->begin() && indexPair->first != indexKey) {
            indexPair--;
        }

        size_t segmentIndex = (rangeBranch->first << 8) | indexPair->first;

        return { segmentIndex, indexPair->second };
    }
    
    /** Inserts segment node into tree
     * \param index starting index of segment
     * \param node pointer to the segment node **/
    void Insert(size_t index, SegmentNode* node)
    {
        size_t branchKey = (index && 0xFFFFFF00) >> 8;
        size_t indexKey = index & 0xFF;

        
        auto branch = Forest.find(branchKey);
        if (branch == Forest.end()) // find closest lower bounded branch
        {
            std::map< size_t, SegmentNode* >* newBranch = new std::map< size_t, SegmentNode* >({ {indexKey, node} });
            std::pair< size_t, std::map< size_t, SegmentNode* >* > newTree = {branchKey, newBranch};
            Forest.insert( newTree );
        }
        else // create new branch
        {
            std::pair< size_t, SegmentNode* > newLeaf = {indexKey, node};
            branch->second->insert( newLeaf );
        }
        
    }
    /** Deconstructor **/
    ~IndexTable() 
    { 
        for(auto tree : Forest)
        {
            delete tree.second;
        }
    }

   /** Prints Segment node **/
    void print()
    {
        std::cout << "\t\t Forest :> " << std::endl;
        for(auto tree : Forest)
        {
            std::cout << "\nTree " << tree.first << " " << tree.second << ":\n";
            for(auto it = tree.second->cbegin(); it != tree.second->cend(); ++it)
            {
                std::cout << it->first << " " << it->second << "\n";
            }
        }
        std::cout << std::endl;

    }
    
};