
#pragma once
#include <vector>
#include <stdint.h>
#include "Genomes/AbstractGenome.h"
#include <cstddef>
#include <algorithm>
#include <map>
#include <unordered_map>

class TetianaGenome : public AbstractGenome {
private:
    // custom properties & functions
    std::vector<std::byte> sites;
    
    std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}}; // {index, {shift, insertion?}}
    std::unordered_map<int, std::vector<std::byte>> segments_log; // {index, {inserted vals}}
    bool empty = false;
    
    //size_t genomeSize;
    
public:
    TetianaGenome(size_t _size);
    ~TetianaGenome() override {
        std::cout << "done" << std::endl;
    }
    
    virtual size_t size() override;
    
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
    
    int get_ind(int ind_curr);

    
    // starting at index, write values in segement over values currently in genome
    virtual void overwrite(size_t index, const std::vector<std::byte>& segment) override;
        
    // starting at index, write values in segement genome between genome[index-1] and genome[index]
    virtual void insert(size_t index, const std::vector<std::byte>& segment) override;
    
    // starting at index, delete segmentSize values from genome starting at genome[index]
    virtual void remove(size_t index, size_t segmentSize) override;
    
    
    std::vector<std::byte> offspring_recon();
    
    // print the whole genome
    virtual void show() override;
    
    
    virtual std::vector<size_t> find_all(std::vector<std::byte>& pattern) override {
        std::vector<size_t> positions;
        
        size_t badMatchTable[256] = {};
        std::fill(std::begin(badMatchTable), std::end(badMatchTable), -1);
        
        for (int i(0); i < pattern.size(); i++) {
            badMatchTable[(size_t)pattern[i]] = pattern.size() - i;
        }
        
        std::size_t skip;
        for (size_t s_index = 0; s_index < (sites.size() - pattern.size())+1; s_index += skip) {
            skip = 0;
            for (int p_index(pattern.size() - 1); p_index >= 0; p_index--) {
                if (sites[s_index + p_index] != pattern[p_index]) {
                    skip = std::max(1, p_index - (int)badMatchTable[(size_t)sites[s_index + p_index]]);
                    break;
                }
            }
            if (skip == 0) {
                positions.push_back(s_index);
                skip = 1;
            }
        }
        return(positions);
    }
};
