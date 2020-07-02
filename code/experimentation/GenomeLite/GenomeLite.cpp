/**
 * \file GenomeLite.cpp
 * \author Victoria Cao
 **/

#include <iostream>

#include "GenomeLite.h"
#include "GeneSegment.h"


void GenomeLite::Reallocate()
{
    Handler* handler = new Handler(this);

    auto gene = std::make_shared< GeneSegment >(size());
    auto node = handler->GetSegment();

    while(node)
    {
        std::memcpy(gene->Gene+handler->GetIndex(), node->Start, node->GetSize());

        handler->NextSegment();
        node = handler->GetSegment();
    }

    delete handler;
    delete Tree;

    Tree = new SegmentTree(gene);
}


/** Makes a delete mutation at current index **/
void GenomeLite::DeleteMutation(size_t index)
{
    Tree->Delete(index);
    if (Tree->Pool->GetSize()-Tree->Pool->GetTail() < 5)
    {
        Reallocate();
    }
}


/** Makes a point mutation at current index
 * \param mutation point mutation to make**/
void GenomeLite::PointMutation(size_t index, Byte* mutation, size_t size)
{
    Tree->Point(index, mutation, size);
    if (Tree->Pool->GetSize()-Tree->Pool->GetTail() < 5)
    {
        Reallocate();
    }
}

/** Makes a point mutation at current index
 * \param mutation point mutation to make**/
void GenomeLite::InsertMutation(size_t index, Byte* mutation, size_t size)
{
    Tree->Insert(index, mutation, size);
    if (Tree->Pool->GetSize()-Tree->Pool->GetTail() < 5)
    {
        Reallocate();
    }
}


/** Resets Handler **/
void GenomeLite::Handler::Reset()
{
    // clear stack and move to Root of tree
    Stack.clear();
    SegmentNode* node = GetRoot();

    while(node) // move to leftmost node and append parent nodes to stack
    {
        Stack.push_back(node);
        node = node->GetLeft(GetTree()->Pool);
    }  
}


/** Increment the mutator **/
void GenomeLite::Handler::Next()
{
    ++NodeIndex;
    ++Index;

    if (NodeIndex == Stack.back()->GetSize())
    {
        auto node = Stack.back()->GetRight(GetTree()->Pool);
        Stack.pop_back();
        while(node)
        {
            Stack.push_back(node);
            node = node->GetLeft(GetTree()->Pool);
        }

        NodeIndex = 0;
    }
}

/** If handler can increment 
 * \return if there is a next node**/
bool GenomeLite::Handler::HasNext()
{
    return Index < GetRoot()->GetWeight();
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
    size_t right = node->GetWeight();    // Right boundary of node
    if (node->GetLeft(GetTree()->Pool))
        left += node->GetLeft(GetTree()->Pool)->GetWeight();
    if (node->GetRight(GetTree()->Pool))
        right -= node->GetRight(GetTree()->Pool)->GetWeight();

    size_t min = 0;     // Left boundary of node's left subtree
    size_t max = node->GetWeight();  // Right boundary of node's right subtree
    
    /// Find the segment that index lies within
    while(index < left || index >= right)
    {
        if (index >= right)
        { 
            min = right;
            node = node->GetRight(GetTree()->Pool);
        }  
        else
        {
            Stack.push_back(node); 

            max = left;
            node = node->GetLeft(GetTree()->Pool);
        }
        
        // Update node's segment boundary
        left = min;
        right = max;
        if (node->GetLeft(GetTree()->Pool))
            left += node->GetLeft(GetTree()->Pool)->GetWeight();
        if (node->GetRight(GetTree()->Pool))
            right -= node->GetRight(GetTree()->Pool)->GetWeight();
    }

    NodeIndex = index-left;
    Stack.push_back(node);
}

/** Prints Handler positions and current node**/
void GenomeLite::Handler::Print()
{
    std::cout << "Global Pos: " << Index << "\t Node Pos: " << NodeIndex << std::endl;
    Stack.back()->Print();
}

/** Increment the mutator **/
void GenomeLite::Handler::NextSegment()
{
    Index += Stack.back()->GetSize();
    auto node = Stack.back()->GetRight(GetTree()->Pool);
    Stack.pop_back();
    while(node)
    {
        Stack.push_back(node);
        node = node->GetLeft(GetTree()->Pool);
    }
}

SegmentNode* GenomeLite::Handler::GetSegment()
{
    if (Stack.size() > 0)
        return Stack.back();
    return nullptr;
}
