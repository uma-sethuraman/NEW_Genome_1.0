/**
 * \file Handler.h
 * \author Victoria Cao
 * 
 * \brief Segment node class for segmentation of data
 * implementation
 **/
#include <cstdint>
#include "Handler.h"
#include "Mutation.h"

/** Resets Handler **/
void Handler::reset()
{
    Segment = Genome->GeneSegments->StartSegment;
    SegmentIterator = Segment->begin();
    Pos = 0;
}


/** Increment the mutator **/
void Handler::next()
{
    SegmentIterator++;
    if (SegmentIterator == Segment->end())
    {
        Pos += Segment->size();
        Segment = Segment->next();
        if (Segment)
            SegmentIterator = Segment->begin();
    }
}

/** Decrement the mutator **/
void Handler::prev()
{
    SegmentIterator--;
    if (SegmentIterator == Segment->begin()--)
    {
        Pos -= Segment->size();
        Segment = Segment->prev();
        SegmentIterator = Segment->end()--;
    }
}


/** Connect two nodes
 * \param left node 
 * \param right node **/
void Handler::Connect(std::shared_ptr<SegmentNode> left, std::shared_ptr<SegmentNode> right)
{
    if (left)
        left->SetNext(right);
    if (right)
        right->SetPrev(left);
}

/** Insertion mutation 
 * \param startMutation address of insertion mutation **/
void Handler::PointMutation(std::shared_ptr< Mutation > mutation)
{
    std::cout << "Point mutation" << std::endl; 

    std::shared_ptr<SegmentNode> pointMutation = std::make_shared<SegmentNode>(mutation->data(), mutation->size());

    if (SegmentIterator.pos() == 0) // Front case
    {  
        /// cut segment
        Segment->TruncateLeft();

        /// connect segments
        Connect(Segment->prev(), pointMutation);
        Connect(pointMutation, Segment);

        /// move head of reader
        Pos++;
        SegmentIterator = Segment->begin();

        /// update starting segment
        if (Segment == Genome->GeneSegments->StartSegment)
            Genome->GeneSegments->StartSegment = pointMutation;
    }
    else if (SegmentIterator.pos() == Segment->size()-1)  // Back case
    {
        /// cut segment 
        Segment->TruncateRight();

        /// connect segments
        Connect(Segment, pointMutation);
        Connect(pointMutation, Segment->next());

        /// move head of reader to next segment
        Pos += Segment->size()+1;
        Segment = pointMutation->next();
    }
    else    // Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(SegmentIterator.pos());
        cutSegment->TruncateLeft();

        /// connect the cut segments
        Connect(Segment, pointMutation);
        Connect(pointMutation, cutSegment);
        Connect(cutSegment, Segment->next());

        /// move head of reader to newly cut segment
        Pos += Segment->size();
        Segment = cutSegment;
        SegmentIterator = Segment->begin();

        /// update number of segments
        Genome->GeneSegments->Size++;
    }

    /// update number of segments
    Genome->GeneSegments->Size++;
    
}


/** Insertion mutation 
 * \param startMutation address of insertion mutation
 * \param sizeMutation size of mutation **/
void Handler::InsertMutation(std::shared_ptr< Mutation > mutation)
{
    std::cout << "Insetion mutation" << std::endl; 

    std::shared_ptr<SegmentNode> insertMutation = std::make_shared<SegmentNode>(mutation->data(), mutation->size());

    if (SegmentIterator.pos() == Segment->size()-1)  // Back case
    {
        /// connect segments
        Connect(Segment, insertMutation);
        Connect(insertMutation, Segment->next());

        /// move head of reader to next segment
        Pos += Segment->size()+1;
        Segment = insertMutation->next();
        if (Segment)
            SegmentIterator = Segment->begin();
    }
    else    //  Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(SegmentIterator.pos()+1);

        /// connect the cut segments
        Connect(Segment, insertMutation);
        Connect(insertMutation, cutSegment);
        Connect(cutSegment, Segment->next());

        /// move head of reader to newly cut segment
        Pos += Segment->size()+1;
        Segment = cutSegment;
        SegmentIterator = Segment->begin();

        /// update number of segments
        Genome->GeneSegments->Size++;
    }

    /// update number of segments
    Genome->GeneSegments->Size++;
    Genome->GeneSegments->SiteCount++;
}


/** Copy mutation 
 * \param insertionMutation address of insertion mutation
 * \param sizeMutation size of mutation **/
void Handler::CopyMutation(Byte* startMutation, size_t sizeMutation)
{
    std::cout << "Copy mutation" << std::endl;
    /// cut segment and remove value

}


/** Insertion mutation 
 * \param sizeMutation size of mutation **/
void Handler::DeleteMutation(size_t sizeMutation)
{
    if (SegmentIterator.pos() == 0) // Front case
    {  
        /// cut segment
        Segment->TruncateLeft();

        /// move head of reader
        SegmentIterator = Segment->begin();
    }
    else if (SegmentIterator.pos() == Segment->size()-1)  // Back case
    {
        /// cut segment 
        Segment->TruncateRight();

        /// move head of reader to next segment
        Pos += Segment->size();
        Segment = Segment->next();
        if (Segment)
            SegmentIterator = Segment->begin();
    }
    else    // Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(SegmentIterator.pos());
        cutSegment->TruncateLeft();

        /// connect the cut segments
        Connect(Segment, cutSegment);
        Connect(cutSegment, Segment->next());

        /// move head of reader to newly cut segment
        Pos += Segment->size();
        Segment = cutSegment;
        SegmentIterator = Segment->begin();

        /// update number of segments
        Genome->GeneSegments->Size++;
    }
    
    /// edit genome properties
    Genome->GeneSegments->SiteCount -= sizeMutation; 
}
