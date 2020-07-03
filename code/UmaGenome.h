#pragma once
#include <vector>
#include <stdint.h>
#include "AbstractGenome.h"
#include <cstddef>
#include <map>

class UmaGenome : public AbstractGenome {
private:
    // custom properties & functions
    std::vector<std::byte> sites;

    int alphabetSize; // to use when randomizing mutation values, not used yet

    std::map<int, std::byte> changelog;
    std::map<int, int> offsetMap;
    int currentGenomeSize; // size of current genome

public:
    UmaGenome(size_t _size);
    UmaGenome(size_t _size, int alph_size);
    ~UmaGenome() override {
        //std::cout << "done" << std::endl;
    }

    std::byte* data() override;

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

    int getLowerBoundOffset(int key);
    void pointMutate(int index, std::byte value);
    void insertMutate(int start, std::vector<std::byte> values);
    void deleteMutate(int start, int size);

    int getAlphabetSize() {
        return alphabetSize;
    }

    int getCurrentGenomeSize() {
        return currentGenomeSize;
    }

    void printChangelog();
    void printOffsetMap();
    std::byte getCurrentGenomeAt(int pos); // random access method
    void reconstructGenome();
};

namespace NK {

    // Represents an organism (genome, score) in a population of organisms
    // Used in NK evaluator
    struct Organism {
        AbstractGenome* genome;
        double score;
    };
}
