/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief Segment node class for segmentation of data
 * implementation
 **/

#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <iostream>


typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class SegmentNode
{
private:
    /// Data variables
	Byte* Start; ///< Start of memory
    size_t Size; ///< Size of segment

    /// Linked list variables
    std::shared_ptr<SegmentNode> Prev = nullptr;
    std::shared_ptr<SegmentNode> Next = nullptr;

public:
    /** (deleted) Default Constructor */
    SegmentNode() = delete;

    /** Constructor
     * \param start beginning of segment
     * \param size size of segment */
    SegmentNode(Byte* start, size_t size) 
        : Start(start), Size(size)
    {
        std::cout << "SegmentNode " << this << std::endl;
        std::cout << "B " << std::hex << (unsigned long)Start << std::endl;
        std::cout << "E " << (unsigned long)Start+Size << std::endl;
        std::cout << std::endl;
    }

    /** Deconstructor **/
    ~SegmentNode() {}

    /** Gets size
     * \returns size of segment **/
    const size_t size() { return Size; }

    /** Gets size
     * \returns size of segment **/
    std::shared_ptr<SegmentNode> next() { return Next; }

    /** Gets size
     * \returns size of segment **/
    std::shared_ptr<SegmentNode> prev() { return Prev; }

    /** Sets size
     * \returns size of segment **/
    void SetNext(std::shared_ptr<SegmentNode> next) { Next = next; }

    /** Setes size
     * \returns size of segment **/
    void SetPrev(std::shared_ptr<SegmentNode> prev) { Prev = prev; }


    void TruncateLeft(size_t cutSize = 1);
    void TruncateRight(size_t cutSize = 1);
    std::shared_ptr<SegmentNode> Cut(size_t index);


    /** Iterator that iterates over the segments */
    class Iter
    {
    private:
        SegmentNode* Segment;   ///< Segments we are iterating over
        size_t Pos;       ///< Position in the Segment

    public:
        /** Constructor
        * \param segment The segment we are iterating over 
        * \param pos Position within segment
         */
        Iter(SegmentNode* segment, size_t pos) : Segment(segment), Pos(pos) {}

        /** Gets current position
         * \return position in segment **/
        const size_t pos() { return Pos; }

        /** Test for end of the iterator
        * \param other Other Iter object to compare against
        * \returns True if we this position equals not equal to the other position */
        bool operator==(const Iter& other) const { return Pos == other.Pos && Segment == other.Segment; }

        /** Test for end of the iterator
        * \param other Other Iter object to compare against
        * \returns True if we this position equals not equal to the other position */
        bool operator!=(const Iter& other) const { return Pos != other.Pos || Segment != other.Segment; }

        /** Get value at current position
         * \returns Value at Pos in the collection */
        Byte operator *() const { return Segment->Start[Pos]; }

        /** Increment the iterator
         * \returns Reference to this iterator */
        const Iter& operator++(int)
        {
            Pos++;
            return *this;
        }

        /** Increment the iterator
         * \returns Reference to this iterator */
        const Iter& operator--(int)
        {
            Pos--;
            return *this;
        }
    };

    /** Get an iterator for the beginning of the collection
     * \returns Iter object at position 0 */
    Iter begin() { return Iter(this, 0); }

    /** Get an iterator for the end of the collection
     * \returns Iter object at position past the end */
    Iter end() { return Iter(this, Size); }

};
