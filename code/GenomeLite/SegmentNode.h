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
#include <vector>

typedef std::byte Byte; // c++17 std::byte doesn't always work

class SegmentPool;

/** Implementation of a genome object **/
class SegmentNode
{
private:
    // friend class SegmentList
    friend class SegmentList;

    /// Data variables
    std::shared_ptr< std::vector<Byte> > Data;
	size_t Start = 0; ///< Start of memory
    size_t Size; ///< Size of data

    size_t Pos;
    long Next = -1;
    long Prev = -1;

public:
    /** Default Constructor */
    SegmentNode() = default;

    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< std::vector<Byte> > data, size_t pos) 
        : Data(data), Size(data->size()), Pos(pos) {}


    /** Constructor
     * \param start beginning of data
     * \param size size of data */
    SegmentNode(std::shared_ptr< std::vector<Byte> > data, size_t pos, size_t start, size_t size) 
        : Data(data), Start(start), Size(size), Pos(pos) {}

    /** Deconstructor **/
    ~SegmentNode() { }

    const size_t GetPos() { return Pos; }
    const size_t GetSize() { return Size; }


    Byte* GetData(size_t index);
    SegmentNode* GetPrev(SegmentPool* pool);
    SegmentNode* GetNext(SegmentPool* pool);
    SegmentNode* GetNode(size_t index, SegmentPool* pool);

    /** Gets use count 
     * \returns use count of the smart pointer **/
    bool Unique() { return Data.use_count() == 1; }

    void SetPrev(SegmentNode* node);
    void SetNext(SegmentNode* node);

    void TruncateLeft(size_t index = 1);
    void TruncateRight(size_t index = 1);
    SegmentNode* Copy(SegmentPool* pool);
    size_t Overwrite(size_t index, const std::vector<std::byte>& segment, size_t start);
    void Print();

};