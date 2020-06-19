/**
 * \file Handler.cpp
 * \author Victoria Cao
 **/

#include <cstdint>
#include "Handler.h"


/** Resets Handler **/
void Handler::reset()
{
    Segment = Genome->GeneSegments->StartSegment;
    SegmentIterator = Segment->begin();
    Pos = 0;
}


/** Increment the mutator **/
void Handler::next()
{
    SegmentIterator++;
    if (SegmentIterator == Segment->end())
    {
        Pos += Segment->size();
        Segment = Segment->next();
        if (Segment)
            SegmentIterator = Segment->begin();
    }
}

/** Decrement the mutator **/
void Handler::prev()
{
    SegmentIterator--;
    if (SegmentIterator == Segment->begin()--)
    {
        Pos -= Segment->size();
        Segment = Segment->prev();
        SegmentIterator = Segment->end()--;
    }
}
