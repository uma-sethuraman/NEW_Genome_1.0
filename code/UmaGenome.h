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

    // mutation rates
    double MR_Point;
    double MR_Insertion;
    double MR_Deletion;

    int alphabetSize;

    struct site_info {
        std::byte site_value;
        int offset;
        bool has_value;
    };

    std::map<int, site_info> changelog;
    int currentGenomeSize; // size of current genome

    void addChangelogEntryCLB(int key, int site_val, bool has_val);

public:
    UmaGenome(size_t _size);
    UmaGenome(size_t _size, double MR_P, double MR_I, double MR_D, int alph_size);
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

    void pointMutate(int index, int value, bool use_params);
    void insertMutate(); // not implemented yet
    void deleteMutate(int start, int size, bool use_params);

    int getAlphabetSize() {
        return alphabetSize;
    }

    int getCurrentGenomeSize() {
        return currentGenomeSize;
    }

    void printChangelog();
    int getCurrentGenomeAt(int pos);
    void reconstructGenome();

};

namespace NK {

    // Represents an organism (genome, score) in a population of organisms
    struct Organism {
        AbstractGenome* genome;
        double score;
    };
}
