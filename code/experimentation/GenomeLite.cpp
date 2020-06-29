/**
 * \file GenomeLite.cpp
 * \author Victoria Cao
 **/

#include <iostream>

#include "GenomeLite.h"
    

/** Resets Handler **/
void GenomeLite::Handler::Reset()
{
    // clear stack and move to Root of tree
    Stack.clear();
    SegmentNode* node = GetRoot();

    while(node) // move to leftmost node and append parent nodes to stack
    {
        Stack.push_back(node);
        node = node->Left;
    }  
}


/** Increment the mutator **/
void GenomeLite::Handler::Next()
{
    NodeIndex++;
    Index++;

    if (NodeIndex == Stack.back()->GetSize())
    {
        auto node = Stack.back()->Right;
        Stack.pop_back();
        while(node)
        {
            Stack.push_back(node);
            node = node->Left;
        }

        NodeIndex = 0;
    }
}

/** If handler can increment 
 * \return if there is a next node**/
bool GenomeLite::Handler::HasNext()
{
    return Index < GetRoot()->Weight;
}

/** Decrement the mutator **/
void GenomeLite::Handler::Prev()
{
}

/** If handler can decrement 
 * \return if there is a prev node**/
bool GenomeLite::Handler::HasPrev()
{
}

/** Moves handler to index 
 * \param index **/
void GenomeLite::Handler::MoveTo(size_t index)
{
    // GetTree()->Print();

    Stack.clear();
    SegmentNode* node = GetRoot();

    /// boundary variables
    size_t left = 0;    // Left boundary of node
    size_t right = node->Weight;    // Right boundary of node
    if (node->Left)
        left += node->Left->Weight;
    if (node->Right)
        right -= node->Right->Weight;

    size_t min = 0;     // Left boundary of node's left subtree
    size_t max = node->Weight;  // Right boundary of node's right subtree
    
    /// Find the segment that index lies within
    while(index < left || index >= right)
    {
        if (index >= right)
        { 
            min = right;
            node = node->Right;
        }  
        else
        {
            Stack.push_back(node); 

            max = left;
            node = node->Left;
        }
        
        // Update node's segment boundary
        left = min;
        right = max;
        if (node->Left)
            left += node->Left->Weight;
        if (node->Right)
            right -= node->Right->Weight;
    }

    NodeIndex = index-left;
    Stack.push_back(node);
}


/** Makes a delete mutation at current index **/
void GenomeLite::Handler::DeleteMutation(size_t index)
{
    GetTree()->Delete(index);
}


/** Makes a point mutation at current index
 * \param mutation point mutation to make**/
void GenomeLite::Handler::PointMutation(size_t index, Byte* mutation, size_t size)
{
    GetTree()->Point(index, mutation, size);
}

/** Makes a point mutation at current index
 * \param mutation point mutation to make**/
void GenomeLite::Handler::InsertMutation(size_t index, Byte* mutation, size_t size)
{
    GetTree()->Insert(index, mutation, size);
}

/** Prints Handler positions and current node**/
void GenomeLite::Handler::Print()
{
    std::cout << "Global Pos: " << Index << "\t Node Pos: " << NodeIndex << std::endl;
    Stack.back()->Print();
}
