/**
 * \file Handler.h
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

/** Handler Handler for mutating over genome **/
class Handler
{
private:
    std::shared_ptr<GenomeLite> Genome; ///< current genome iterating on
    std::shared_ptr<SegmentNode> Segment;    ///< current SegmentNode iterating on
    size_t Pos = 0;  ///< position up until Segment

    SegmentNode::Iter SegmentIterator;

public:
    /** (deleted) default Constructor **/
    Handler() = delete;

    /** Constructor
     * \param genome Genome Mutation on */
    Handler(std::shared_ptr<GenomeLite> genome) 
        : Genome(genome), Segment(genome->GeneSegments->StartSegment), SegmentIterator(genome->GeneSegments->StartSegment->begin()) {}

    /** Deconstructor **/
    ~Handler() {}

    /** Gets current position
    * \return position in segment **/
    const size_t index() { return Pos+SegmentIterator.pos(); }

    /** Get value at current position
     * \returns Value at Pos in the collection */
    Byte operator *() const { return *SegmentIterator; }

    void reset();
    void next();
    void prev();
    void Connect(std::shared_ptr<SegmentNode> left, std::shared_ptr<SegmentNode> right);
    template < typename T >
    void PointMutation(T value);
    template < typename T >
    void InsertMutation(T value);
    void CopyMutation(Byte* startMutation, size_t sizeMutation = 1);
    void DeleteMutation(size_t sizeMutation = 1);
};