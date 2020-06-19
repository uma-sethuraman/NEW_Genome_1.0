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
template < typename T >
void Handler::PointMutation(T value)
{
    std::cout << "Point mutation" << std::endl; 

    Mutation mutation(value);
    std::shared_ptr<SegmentNode> pointMutation = std::make_shared<SegmentNode>(mutation.data(), mutation.size());

    if (SegmentIterator.pos() == 0) // Front case
    {  
        /// cut segment
        Segment->TruncateLeft();

        /// connect segments
        Connect(Segment->prev(), pointMutation);
        Connect(pointMutation, Segment);

        /// move head of reader
        SegmentIterator = Segment->begin();
    }
    else if (SegmentIterator.pos() == Segment->size()-1)  // Back case
    {
        /// cut segment 
        Segment->TruncateRight();

        /// connect segments
        Connect(Segment, pointMutation);
        Connect(pointMutation, Segment->next());

        /// move head of reader to next segment
        Segment = Segment->next();
    }
    else    // Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(SegmentIterator.pos());

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
template < typename T >
void Handler::InsertMutation(T value)
{

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


/// Defining template functions
template void Handler::PointMutation(u_int8_t value);
template void Handler::PointMutation(u_int16_t value);
template void Handler::PointMutation(u_int32_t value);
template void Handler::PointMutation(u_int64_t value);
template void Handler::PointMutation(int value);
template void Handler::PointMutation(long value);
template void Handler::PointMutation(long long value);
template void Handler::PointMutation(char value);

template void Handler::InsertMutation(u_int8_t value);
template void Handler::InsertMutation(u_int16_t value);
template void Handler::InsertMutation(u_int32_t value);
template void Handler::InsertMutation(u_int64_t value);
template void Handler::InsertMutation(int value);
template void Handler::InsertMutation(long value);
template void Handler::InsertMutation(long long value);
template void Handler::InsertMutation(char value);