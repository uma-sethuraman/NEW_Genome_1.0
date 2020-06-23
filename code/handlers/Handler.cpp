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

/** Moves handler to index 
 * \param index **/
void Handler::moveTo(size_t index)
{
    // get segment index is located in
    auto segmentInfo = Genome->GeneSegments->Table->Search(index);

    Segment = segmentInfo.second;
    NodeIndex = segmentInfo.first-index;
    GlobalIndex = index;

}
