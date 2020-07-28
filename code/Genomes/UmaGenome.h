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
    std::map<int, std::byte> changelog;
    std::map<int, int> offsetMap;
    bool mutationsOccurred; // false if no mutations have occurred on sites yet
    size_t currentGenomeSize; // current genome size

public:
    UmaGenome(size_t _size);
    ~UmaGenome() override {
        //std::cout << "done" << std::endl;
    }

    // copy constructor
    UmaGenome(const UmaGenome &genome) {
        sites = genome.sites;
        changelog = genome.changelog;
        offsetMap = genome.offsetMap;
        mutationsOccurred = genome.mutationsOccurred;
        currentGenomeSize = genome.currentGenomeSize;
    }

    std::byte* data(size_t index = 0, size_t byteSize = 0) override;

    virtual size_t size() override;
    virtual void resize(size_t new_size) override;

    virtual AbstractGenome* clone(bool forceCopy = false) override;

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

    void pointMutate(size_t index, std::byte value);
    virtual void overwrite(size_t index, const std::vector<std::byte>& segment) override;
    virtual void insert(size_t index, const std::vector<std::byte>& segment) override;
    virtual void remove(size_t index, size_t segmentSize) override;
    virtual void show() override;

    int getLowerBoundOffset(int key);
    void printChangelog();
    void printOffsetMap();
    std::vector<std::byte> getCurrentGenome();
    std::byte getCurrentGenomeAt(int pos); // random access method
};
