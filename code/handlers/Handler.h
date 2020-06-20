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

#include "../GenomeLite.h"
#include "../SegmentList.h"
#include "../SegmentNode.h"
#include "../GeneSegment.h"

/** Handler Handler for mutating over genome **/
class Handler
{
protected:
    GenomeLite* Genome; ///< current genome iterating on
    SegmentNode* Segment;    ///< current SegmentNode iterating on
    size_t GlobalIndex = 0;  ///< position in segmentList
    size_t NodeIndex = 0;   ///< position in current Node

public:
    /** (deleted) default Constructor **/
    Handler() = delete;

    /** Constructor
     * \param genome Genome Mutation on */
    Handler(GenomeLite* genome) 
        : Genome(genome), Segment(genome->GeneSegments->Head) {}

    /** Deconstructor **/
    ~Handler() {}

    /** Gets current position
    * \return position in segment **/
    const size_t index() { return GlobalIndex; }

    /** Get value at current position
     * \returns Value at Pos in the collection */
    const Byte operator *() const { return Segment->getData(NodeIndex); }

    void reset();
    void next();
    void prev();

    void print()
    {
        std::cout << "Global Pos: " << GlobalIndex << "\t Node Pos: " << NodeIndex << std::endl;
        Segment->print();
    }
};