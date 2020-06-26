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

#include "AbstractHandler.h"

#include "AbstractGenome.h"
#include "SegmentTree.h"
#include "SegmentNode.h"


typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genomeLite object **/
class GenomeLite : public AbstractGenome
{
private:
    SegmentTree Tree;

public:
    /** Constructor
     * \param  genome Parent genome the GenomeLite is constructed from*/
	GenomeLite(AbstractGenome* genome) 
        : AbstractGenome(genome->size()), Tree(genome->data(), genome->size()) {}

    /** Deconstructor **/
	~GenomeLite() {}

    /** Gets size 
     * \returns size of genome **/
    virtual size_t size() override { return Tree.GetSiteCount(); }

    /** Prints Segment list **/
    void Print() { Tree.Print(); }

    /** GenomeLiteHandler GenomeLiteHandler for mutating over genome **/
    class Handler : public AbstractHandler
    {
    private:
        GenomeLite* Genome;
        SegmentNode* CurrentNode;
        size_t NodeIndex = 0;   ///< position in current Node

        /// for iterating over tree
        std::vector< SegmentNode* > Stack;

    public:
        /** (deleted) default Constructor **/
        Handler() = delete;

        /** Constructor
         * \param genome Genome Mutation on */
        Handler(GenomeLite* genome)
            : Genome(genome), CurrentNode(Genome->Tree.Root) { Reset(); }

        /** Deconstructor **/
        ~Handler() {}

        /** Get value at current position
         * \returns Value at Pos in the collection */
        virtual const Byte operator *() const override { return CurrentNode->GetData(NodeIndex); }

        virtual void Reset() override;
        virtual void Next() override;
        virtual void Prev() override;
        virtual void MoveTo(size_t index) override;
        virtual void Print() override;
    };

    virtual AbstractHandler CreateHandler() override 
    { 
        AbstractHandler newHandler = Handler(this);
        return newHandler; 
    }

};
