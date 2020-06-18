/**
 * \file Genome.h
 * \author Victoria Cao
 * 
 * \brief Base class for a simple genome class using SegmentNode 
 * implementation
 **/

#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>
#include "AbstractGenome.h"

typedef char Byte; // c++17 std::byte doesn't always work


/** Implementation of a genom object **/
class Genome : public AbstractGenome 
{
private:
	Byte* gene; ///< array gene of raw data

public:
    /** Constructor
     * \param _size Size of gene */
	Genome(size_t _size) : AbstractGenome(_size)
    {
        gene = new Byte[size_];
    }

    /** Deconstructor **/
	~Genome() override 
    {
		delete[] gene;
	}

    /** Gets pointer to data
     * \return pointer to gene */
	std::byte* data() override
    {
        return reinterpret_cast<std::byte*>(gene);
    }

	virtual void resize(size_t new_size) override 
    {}

	virtual int initGeneSet(std::vector<GeneDefinition> geneInfo) override 
    {
		// geneInfo is a struct of GeneDefinition. Genome will localize the geneInfo and
		// return a key that can be used to access these genes in the future

		// temporary return
		return(1);
	}

	virtual std::unordered_map<std::string, std::vector<size_t>> getGenePositions(int key) override 
    {
		// return a map of vector where each map key is the name indicated in GeneDefinition,
		//and the vector is the list of start codons associated with that name.

		// temporary return
		std::unordered_map<std::string, std::vector<size_t>> geneMap = { {"det", {0}},{"prob",{ 10,20}} };
		return(geneMap);
	}
};
