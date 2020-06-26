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
    SegmentNode* node = Genome->Tree.Root;

    while(node) // move to leftmost node and append parent nodes to stack
    {
        Stack.push_back(node);
        node = node->Left;
    }  
}


/** Increment the mutator **/
void GenomeLite::Handler::Next()
{
}

/** Decrement the mutator **/
void GenomeLite::Handler::Prev()
{
}

/** Moves handler to index 
 * \param index **/
void GenomeLite::Handler::MoveTo(size_t index)
{
}


/** Prints Handler positions and current node**/
void GenomeLite::Handler::Print()
{
    std::cout << "Global Pos: " << Index << "\t Node Pos: " << NodeIndex << std::endl;
    CurrentNode->Print();
}
