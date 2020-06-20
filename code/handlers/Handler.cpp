/**
 * \file Handler.cpp
 * \author Victoria Cao
 **/

#include <cstdint>
#include "Handler.h"


/** Resets Handler **/
void Handler::reset()
{
    Segment = Genome->GeneSegments->Head;
    GlobalIndex = 0;
    NodeIndex = 0;
}


/** Increment the mutator **/
void Handler::next()
{
    NodeIndex++;
    GlobalIndex++;
    if (NodeIndex == Segment->size())
    {
        NodeIndex = 0;
        Segment = Segment->next();
    }
}

/** Decrement the mutator **/
void Handler::prev()
{
    NodeIndex--;
    GlobalIndex--;
    if (NodeIndex == 0)
    {
        NodeIndex = 0;
        Segment = Segment->prev();
    }
}
