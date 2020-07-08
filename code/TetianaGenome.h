#pragma once
#include <vector>
#include <stdint.h>
#include "AbstractGenome.h"
#include <cstddef>
#include <random> // random_device etc

#include <map>
#include <unordered_map>

class TetianaGenome : public AbstractGenome {
private:
	// custom properties & functions
	std::vector<std::byte> sites;
    std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
    std::unordered_map<int, std::vector<std::byte>> insertions;
    
public:
	TetianaGenome(size_t _size);
	~TetianaGenome() override {
		std::cout << "done" << std::endl;
	}

	std::byte* data(size_t index = 0, size_t byteSize = 0) override;

	virtual void resize(size_t new_size) override;

	virtual int initGeneSet(std::vector<GeneDefinition> geneInfo) override {
		// geneInfo is a struct of GeneDefinition. Genome will localize the geneInfo and
		// return a key that can be used to access these genes in the future

		// temporary return
		return(1);
	}

	virtual std::unordered_map<std::string, std::vector<size_t>> getGenePositions(int key) override {
		// return a map of vector where each map key is the name indicated in GeneDefinition,
		//and the vector is the list of start codons associated with that name.

		// temporary return
		std::unordered_map<std::string, std::vector<size_t>> geneMap = { {"det", {0}},{"prob",{ 10,20}} };
		return(geneMap);
	}
    
    virtual void mutate() override;
    
    // starting at index, write values in segement genome between genome[index-1] and genome[index]
    virtual void insert(size_t index, std::vector<std::byte> segment) override;
    

    
};
