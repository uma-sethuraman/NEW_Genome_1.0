#pragma once
#include <stdint.h>
#include <iostream>
#include<unordered_map>
#include <cstddef>
#include <list>

class AbstractGenome {
protected:
    size_t size_; // size should be size of memory when 

public:
    struct GeneDefinition {
        std::string name;
        std::vector<std::byte> startCodon; //vector of bytes
        size_t geneSize; // size in bytes
    };

    AbstractGenome() = delete;

    AbstractGenome(size_t _size) :size_(_size) {}

    virtual ~AbstractGenome() {} // member function that you expect to be redefined in derived classes
    
    virtual size_t size() {return size_;}
    
    virtual void resize(size_t new_size) {size_ = new_size;};

    virtual std::byte* data(size_t index = 0, size_t byteSize = 0) {
        std::cout << "data has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual AbstractGenome* clone() {
        std::cout << "clone has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void mutate() {
        std::cout << "mutate has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void printChangelog() {
        std::cout << "printChangelog has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void generateGenome(AbstractGenome* genome) {
        std::cout << "generateGenome has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void initalize() {
        std::cout << "initalize has not been written for this genome class" << std::endl;
        exit(1);
    }

    // return an int key to the genes in question
    virtual int initGeneSet(std::vector<GeneDefinition> geneInfo) {
        std::cout << "initGeneView has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual std::unordered_map<std::string,std::vector<size_t>> getGenePositions(int key) {
        std::cout << "getGenePositions has not been written for this genome class" << std::endl;
        exit(1);
    }

    // new functions

    // starting at index, write values in segment over values currently in genome
    virtual void overwrite(size_t index, std::vector<std::byte> segment) {
        std::cout << "overwrite has not been written for this genome class" << std::endl;
        exit(1);
    }

    // starting at index, write values in segment genome between genome[index-1] and genome[index]
    virtual void insert(size_t index, std::vector<std::byte> segment) {
        std::cout << "insert vector has not been written for this genome class" << std::endl;
        exit(1);
    }

    // starting at index, delete segmentSize values from genome starting at genome[index]
    virtual void remove(size_t index, size_t segmentSize) {
        std::cout << "remove has not been written for this genome class" << std::endl;
        exit(1);
    }
};

namespace GN {

    template <class T>
    auto genomeRead(AbstractGenome* genome, size_t index) -> T& {
        return reinterpret_cast<T*>(genome->data() + index)[0];
    }

    template <class T>
    auto genomeWrite(AbstractGenome* genome, size_t index, const T& value) -> void {
        memcpy(genome->data() + index, &value, sizeof(T));
    }

    // ==============================================
    // python-like named arguments
    // ==============================================

    struct GenomeReadParams {
        AbstractGenome* genome;
        size_t index;
    };
    template <class T>
    auto genomeRead(const GenomeReadParams& params) -> T& {
        return genomeRead<T>(params.genome, params.index);
    }

    template <class T>
    struct GenomeWriteParams {
        AbstractGenome* genome;
        size_t index;
        const T& value;
    };
    template <class T>
    auto genomeWrite(const GenomeWriteParams<T>& params) -> void {
        genomeWrite<T>(params.genome, params.index, params.value);
    }
}
