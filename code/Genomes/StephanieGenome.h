#pragma once
#include <vector>
#include <stdint.h>
#include "AbstractGenome.h"
#include <cstddef>
#include <map>
#include <iostream>

class StephanieGenome : public AbstractGenome {
private:
	struct ChangelogStruct {
		size_t insertOffset;
		size_t removeOffset;
		std::byte value;
	};
	bool genomeGenerated = false;
	bool mutationFlag = false;
	size_t genomeSize;
	std::map<size_t, ChangelogStruct> changelog;
	std::vector<std::byte> modifiedSites;
	std::vector<std::byte> sites;

public:
	StephanieGenome(size_t _size);

	~StephanieGenome() override {
		//std::cout << "\ndone" << std::endl;
	}

	StephanieGenome(const StephanieGenome& genome) {
		sites = genome.sites;
		changelog = genome.changelog;
		modifiedSites = genome.modifiedSites;
		genomeSize = genome.genomeSize;
	}


	std::byte* data(size_t index = 0, size_t byteSize = 0) override;

	virtual size_t size() override;

	virtual void resize(size_t new_size) override;

	virtual AbstractGenome* clone(bool forceCopy = false) override;

	virtual void overwrite(size_t index, const std::vector<std::byte>& segment);

	virtual void insert(size_t index, const std::vector<std::byte>& segment);

	virtual void remove(size_t index, size_t segmentSize) override;

	virtual void show() override;

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

	void generateNewGenome();

	void addToChangelog(size_t index, size_t insert, size_t remove, std::byte value);

	void printChangelog();

	void shiftChangelog(bool shiftLeft, std::vector<size_t>& sitesAffected, size_t offset);

};