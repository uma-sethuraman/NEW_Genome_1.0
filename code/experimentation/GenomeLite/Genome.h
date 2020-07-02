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


#include "../AbstractGenome.h"

typedef char Byte; // c++17 Byte doesn't always work


/** Implementation of a genom object **/
class Genome : public AbstractGenome 
{
private:
	Byte* gene; ///< array gene of raw data

public:
    /** Constructor
     * \param size Size of gene */
	Genome(size_t size) : AbstractGenome(size) { gene = new Byte[size_]; }

    /** Deconstructor **/
	~Genome() override { delete[] gene;	}

    /** Gets pointer to data
     * \return pointer to gene */
	Byte* data() override { return reinterpret_cast<Byte*>(gene); }

    /** Resizes the Genome
     * \param newSize new size of genome **/
	virtual void resize(size_t newSize) override {}

    /** Initialize a gene set 
     * 	geneInfo is a struct of GeneDefinition. Genome will localize the geneInfo and
     * return a key that can be used to access these genes in the future
     * \param geneInfo
     * \return empty **/

	virtual int initGeneSet(std::vector<GeneDefinition> geneInfo) override 
    {
		return(1);
	}

    /** Gets map of gene positions
     * return a map of vector where each map key is the name indicated in GeneDefinition,
     * and the vector is the list of start codons associated with that name.
     * \param key
     * \return temp **/
	virtual std::unordered_map<std::string, std::vector<size_t>> getGenePositions(int key) override 
    {
		std::unordered_map<std::string, std::vector<size_t>> geneMap = { {"det", {0}},{"prob",{ 10,20}} };
		return(geneMap);
	}
};
