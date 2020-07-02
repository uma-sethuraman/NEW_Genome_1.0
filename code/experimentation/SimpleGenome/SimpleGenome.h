/**
 * \file SimpleGenome.h
 * \author Victoria Cao
 * 
 * \brief Base class for a simple genome class using SegmentNode 
 * implementation
 **/

#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>


#include "../AbstractGenome.h"

typedef char Byte; // c++17 Byte doesn't always work


/** Implementation of a genom object **/
class SimpleGenome : public AbstractGenome 
{
private:
	std::vector<Byte> gene; ///< array gene of raw data

public:
    /** Constructor
     * \param size Size of gene */
	SimpleGenome(size_t size) : AbstractGenome(size) { gene.resize(size); }

    /** Deconstructor **/
	~SimpleGenome() override {}

    virtual Byte* data() {
        return &gene[0];
    }

    virtual void DeleteMutation(size_t index) override
    {
        gene.erase(gene.begin()+index);
    }

    virtual void PointMutation(size_t index, Byte* mutation, size_t size) override
    {
        for (size_t i(0); i < size; i++)
        {
            gene[index+i] = *(mutation+i);
        }
    }

    virtual void InsertMutation(size_t index, Byte* mutation, size_t size) override
    {
        for (size_t i(0); i < size; i++)
        {
            gene.insert(gene.begin()+index+i, *(mutation+i));
        }
    }

};
