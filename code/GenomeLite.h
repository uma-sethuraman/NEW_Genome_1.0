/**
 * \file GenomeLite.h
 * \author Victoria Cao
 * 
 * \brief Base class for a simple GenomeLite class using SegmentNode 
 * implementation
 **/

#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <iostream>

#include "AbstractGenome.h"
#include "SegmentNode.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genomeLite object **/
class GenomeLite
{
private:
	std::shared_ptr<AbstractGenome> ParentGenome; ///< pointer to parent genome
    std::shared_ptr<SegmentNode> StartSegment; ///< start to linked list of segments

    size_t Size;   ///< size of genome

public:
    /** Constructor
     * \param  genome Parent genome the GenomeLite is constructed from*/
	GenomeLite(std::shared_ptr<AbstractGenome> genome) 
        : ParentGenome(genome), Size(genome->size())
    {
        std::cout << "GenomeLite " << std::endl;
        std::cout << "start " << std::hex << (unsigned long)genome->data() << std::endl;
        std::cout << "end " << (unsigned long)genome->data()+genome->size() << std::endl;
        std::cout << std::endl;

        StartSegment = std::make_shared<SegmentNode>(genome->data(), genome->size());
    }

    /** Deconstructor **/
	~GenomeLite() {}

    /** Gets size 
     * \returns size of genome **/
    size_t size() { return Size; }


    /** Handler Handler for mutating over genome **/
    class Handler
    {
    private:
        GenomeLite* Genome; ///< current genome iterating on
        std::shared_ptr<SegmentNode> Segment;    ///< current SegmentNode iterating on
        size_t Pos;  ///< position up until Segment

        SegmentNode::Iter SegmentIterator;

    public:
        /** (deleted) default Constructor **/
        Handler() = delete;

        /** Constructor
         * \param genome Genome Mutation on */
        Handler(GenomeLite* genome, size_t pos) 
            : Genome(genome), Segment(genome->StartSegment), Pos(pos), SegmentIterator(genome->StartSegment->begin())
        {}

        /** Deconstructor **/
        ~Handler() {}

        /** Gets current position
        * \return position in segment **/
        const size_t index()
        {
            return Pos+SegmentIterator.pos();
        }


        /** Connect two nodes
         * \param left node 
         * \param right node **/
        void Connect(std::shared_ptr<SegmentNode> left, std::shared_ptr<SegmentNode> right)
        {
            if (left)
                left->SetNext(right);
            if (right)
                right->SetPrev(left);
        }

        /** Insertion mutation 
         * \param startMutation address of insertion mutation **/
        void PointMutation(Byte* startMutation)
        {

        }


        /** Insertion mutation 
         * \param startMutation address of insertion mutation
         * \param sizeMutation size of mutation **/
        void InsertMutation(Byte* startMutation, size_t sizeMutation)
        {

        }

        /** Copy mutation 
         * \param insertionMutation address of insertion mutation
         * \param sizeMutation size of mutation **/
        void CopyMutation(Byte* startMutation, size_t sizeMutation)
        {

        }


        /** Insertion mutation 
         * \param sizeMutation size of mutation **/
        void DeleteMutation(size_t sizeMutation = 1)
        {
            std::cout << "delete mutation" << std::endl;
            /// cut segment and remove value
            auto cutSegments = Segment->Subdivide(SegmentIterator.pos());
            auto firstSegment = cutSegments.first;
            auto secondSegment = cutSegments.second;
            secondSegment->TruncateLeft(sizeMutation);

            /// connect the cut segments
            Connect(Segment->prev(), firstSegment);
            Connect(firstSegment, secondSegment);
            Connect(secondSegment, Segment->next());

            /// move head of reader to newly cut segment
            Segment = secondSegment;
            Pos += firstSegment->size();
            SegmentIterator = secondSegment->begin();

            /// edit genome properties
            Genome->Size -= sizeMutation; 
            if (Segment == Genome->StartSegment) 
                Genome->StartSegment = firstSegment;
        }

        /** Get value at current position
         * \returns Value at Pos in the collection */
        Byte operator *() const 
        { 
            return *SegmentIterator; 
        }

        /** Test for end of the iterator
        * \param other Other Iter object to compare against
        * \returns True if we this position equals not equal to the other position */
        bool operator==(const Handler& other) const
        {
            return Pos == other.Pos && Segment == other.Segment;
        }


        /** Increment the mutator
         * \returns Reference to this mutator */
        const Handler& operator++(int)
        {
            SegmentIterator++;
            if (SegmentIterator == Segment->end())
            {
                Pos += Segment->size();
                Segment = Segment->next();
                SegmentIterator = Segment->begin();
            }
            return *this;
        }

        /** Increment the mutator
         * \returns Reference to this mutator */
        const Handler& operator--(int)
        {
            SegmentIterator++;
            if (SegmentIterator == Segment->end())
            {
                Pos -= Segment->size();
                Segment = Segment->prev();
                SegmentIterator = Segment->end()--;
            }
            return *this;
        }
    };

    /** Get an iterator for the beginning of the collection
     * \returns Iter object at position 0 */
    Handler begin() { return Handler(this, 0); }

  
};
