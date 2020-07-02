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

#include "../AbstractHandler.h"

#include "../AbstractGenome.h"
#include "SegmentTree.h"
#include "SegmentNode.h"


typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genomeLite object **/
class GenomeLite : public AbstractGenome
{
private:
    SegmentTree* Tree;

    void Reallocate();

public:
    /** Constructor
     * \param  genome Parent genome the GenomeLite is constructed from*/
	GenomeLite(AbstractGenome* genome) 
        : AbstractGenome(genome->size()), Tree(new SegmentTree(genome->data(), genome->size(), true)) {}

    /** Constructor
     * \param gene Parent genome the GenomeLite is constructed from*/
	GenomeLite(Byte* gene, size_t size) 
        : AbstractGenome(size), Tree(new SegmentTree(gene, size)) {}

    /** Copy constructor
     * \param genome to copy from **/
    GenomeLite(const GenomeLite &genome)
        : AbstractGenome(genome.size_), Tree(new SegmentTree(*genome.Tree)) {}


    virtual AbstractGenome* Clone() override
    {
        return new GenomeLite(*this);
    }

    /** Deconstructor **/
	~GenomeLite() { delete Tree; }

    /** Gets size 
     * \returns size of genome **/
    virtual size_t size() override { return Tree->GetSiteCount(); }

    /** Prints Segment list **/
    void Print() { Tree->Print(); }


    virtual void DeleteMutation(size_t index) override;
    virtual void PointMutation(size_t index, Byte* mutation, size_t size) override;
    virtual void InsertMutation(size_t index, Byte* mutation, size_t size) override;


    /** GenomeLite Handler for mutating over genome **/
    class Handler : public AbstractHandler
    {
    private:
        GenomeLite* Genome; ///< current genome
        size_t NodeIndex = 0;   ///< position in current Node
        std::vector< SegmentNode* > Stack;  ///< iteration stack


        /** Gets tree from genome
         * \return tree **/
        SegmentTree* GetTree() { return Genome->Tree; }

        /** Gets root of tree from genome 
         * \return root **/
        SegmentNode* GetRoot() { return Genome->Tree->Root; }

    public:
        /** (deleted) default Constructor **/
        Handler() = delete;

        /** Constructor
         * \param genome Genome Mutation on */
        Handler(GenomeLite* genome)
            : Genome(genome) { Reset(); }

        /** Destructor **/
        ~Handler() override {};

        /** Get value at current position
         * \returns Value at Pos in the collection */
        virtual const Byte Data() const override { return Stack.back()->GetData(NodeIndex); }

        virtual void Reset() override;
        virtual void Next() override;
        virtual bool HasNext() override;
        virtual void Prev() override;
        virtual bool HasPrev() override;
        virtual void MoveTo(size_t index) override;
        virtual void Print() override;


        void NextSegment();
        bool HasNextSegment();
        SegmentNode* GetSegment();
    };


    /** Creates handler for genome 
     * \return handler for this class **/
    virtual AbstractHandler* NewHandler() override 
    { 
        AbstractHandler* newHandler = new Handler(this);
        return newHandler; 
    }

};
