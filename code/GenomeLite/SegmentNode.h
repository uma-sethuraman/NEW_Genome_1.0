/**
 * \file SegmentNode.h
 * \author Victoria Cao
 * 
 * \brief Segment node struct for segmentation of data
 * implementation
 **/

#pragma once
#include <cstdint>
#include <cstddef>
#include <memory>
#include <vector>

typedef std::byte Byte; // c++17 std::byte doesn't always work

/** Implementation of a genome object **/
struct SegmentNode
{
    /// Data variables
    std::shared_ptr< std::vector<Byte> > Data;
	size_t Start = 0; ///< Start of memory
    size_t Size; ///< Size of data

    size_t Pos;
    size_t Next = 0;
    size_t Prev = 0;

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
    SegmentNode(const SegmentNode& node, size_t pos) 
        : Data(node.Data), Start(node.Start), Size(node.Size), Pos(pos), Next(node.Next), Prev(node.Prev) {}

    /** Deconstructor **/
    ~SegmentNode() { }

};