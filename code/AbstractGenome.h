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

    virtual std::byte* data() {
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

    virtual void insertMutation(size_t index, std::vector<std::byte> values) {
        std::cout << "insertMutation has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void insertMutation(size_t index, std::byte value) {
        std::cout << "insertMutation has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void deleteMutation(size_t index, int delta) {
        std::cout << "deleteMutation has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void pointMutation(size_t index, std::byte value) {
        std::cout << "pointMutation has not been written for this genome class" << std::endl;
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

    // starting at index, write values in segement over values currently in genome
    virtual void overwrite(size_t index, std::vector<std::byte> segment) {
        std::cout << "overwrite has not been written for this genome class" << std::endl;
        exit(1);
    }

    // starting at index, write values in segement genome between genome[index-1] and genome[index]
    virtual void insert(size_t index, std::vector<std::byte> segment) {
        std::cout << "insert has not been written for this genome class" << std::endl;
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
