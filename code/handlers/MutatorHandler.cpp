/**
 * \file MutatorHandler.cpp
 * \author Victoria Cao
 **/

#include <cstdint>
#include "MutatorHandler.h"


/** Connect two nodes
 * \param left node 
 * \param right node **/
void MutatorHandler::Connect(SegmentNode*  left, SegmentNode* right)
{
    if (left)
        left->SetNext(right);
    if (right)
        right->SetPrev(left);
}

/** Insertion mutation 
 * \param startMutation address of insertion mutation **/
void MutatorHandler::PointMutation(std::shared_ptr< GeneSegment > mutation)
{
    SegmentNode* pointMutation = new SegmentNode(mutation);

    if (NodeIndex == 0) // Front case
    {  
        /// cut segment
        Segment->TruncateLeft();

        /// connect segments
        Connect(Segment->prev(), pointMutation);
        Connect(pointMutation, Segment); 

        /// update starting segment
        if (Segment == Genome->GeneSegments->Head)
            Genome->GeneSegments->Head = pointMutation;

        /// move handler to point mutation
        Segment = Segment->prev();
    }
    else if (NodeIndex == Segment->size()-1)  // Back case
    {
        /// cut segment 
        Segment->TruncateRight();

        /// connect segments
        Connect(pointMutation, Segment->next());
        Connect(Segment, pointMutation);

        /// move handler to point mutatoin
        Segment = Segment->next();

    }
    else     // Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(NodeIndex);
        cutSegment->TruncateLeft();

        /// connect the cut segments
        Connect(cutSegment, Segment->next());
        Connect(Segment, pointMutation);
        Connect(pointMutation, cutSegment);

        /// move handler to point mutatoin
        Segment = Segment->next();
        NodeIndex = 0;

        /// update number of segments
        Genome->GeneSegments->Size++;
    }

    /// update number of segments
    Genome->GeneSegments->Size++;
    
}


/** Insertion mutation 
 * \param startMutation address of insertion mutation
 * \param sizeMutation size of mutation **/
void MutatorHandler::InsertMutation(std::shared_ptr< GeneSegment > mutation)
{
    SegmentNode* insertMutation = new SegmentNode(mutation);

    if (NodeIndex == Segment->size()-1)  // Back case
    {
        /// connect segments
        Connect(insertMutation, Segment->next());
        Connect(Segment, insertMutation);
    }
    else    //  Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(NodeIndex+1);

        /// connect the cut segments
        Connect(cutSegment, Segment->next());
        Connect(Segment, insertMutation);
        Connect(insertMutation, cutSegment);

        /// update number of segments
        Genome->GeneSegments->Size++;
    }

    /// move handler to insertion mutation
    GlobalIndex++;
    NodeIndex = 0;
    Segment = Segment->next();

    /// update number of segments
    Genome->GeneSegments->Size++;
    Genome->GeneSegments->SiteCount++;
}


/** Copy mutation 
 * \param insertionMutation address of insertion mutation
 * \param sizeMutation size of mutation **/
void MutatorHandler::CopyMutation(Byte* startMutation, size_t sizeMutation)
{
    std::cout << "Copy mutation" << std::endl;
    /// cut segment and remove value

}


/** Insertion mutation 
 * \param sizeMutation size of mutation **/
void MutatorHandler::DeleteMutation(size_t sizeMutation)
{
    if (NodeIndex == 0) // Front case
    {  
        /// cut segment
        Segment->TruncateLeft();
    }
    else if (NodeIndex == Segment->size()-1)  // Back case
    {
        /// cut segment 
        Segment->TruncateRight();

        NodeIndex--;
    }
    else    // Middle case
    {
        /// cut segment in two
        auto cutSegment = Segment->Cut(NodeIndex);
        cutSegment->TruncateLeft();

        /// connect the cut segments
        Connect(cutSegment, Segment->next());
        Connect(Segment, cutSegment);
  

        /// move head of reader to newly cut segment
        NodeIndex--;

        /// update number of segments
        Genome->GeneSegments->Size++;
    }
    
    /// edit genome properties
    Genome->GeneSegments->SiteCount -= sizeMutation; 
}
