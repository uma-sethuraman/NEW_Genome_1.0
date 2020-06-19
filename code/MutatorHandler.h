/**
 * \file MutatorHandler.h
 * \author Victoria Cao
 * 
 * \brief Segment node class for segmentation of data
 * implementation
 **/

#include <memory>
#include <iostream>
#include <cstddef>

#include "GenomeLite.h"
#include "SegmentList.h"
#include "SegmentNode.h"

#include "Handler.h"

/** MutatorHandler MutatorHandler for mutating over genome **/
class MutatorHandler : public Handler
{
public:
    /** (deleted) default Constructor **/
    MutatorHandler() = delete;

    /** Constructor
     * \param genome Genome GeneSegment on */
    MutatorHandler(std::shared_ptr<GenomeLite> genome) : Handler(genome) {} 

    /** Deconstructor **/
    ~MutatorHandler() {}

    void Connect(std::shared_ptr<SegmentNode> left, std::shared_ptr<SegmentNode> right);
    void PointMutation(std::shared_ptr< GeneSegment > mutation);
    void InsertMutation(std::shared_ptr< GeneSegment > mutation);
    void CopyMutation(Byte* startMutation, size_t sizeMutation = 1);
    void DeleteMutation(size_t sizeMutation = 1);
};