#pragma once
#include <stdint.h>
#include <iostream>
#include<unordered_map>
#include <cstddef>

class AbstractGenome {
protected:
    size_t size_; // size should be size of memory when 

public:
    struct GeneDefinition {
        std::string name;
        std::vector<std::byte> startCodon;
        size_t geneSize; // size in bytes
    };

    AbstractGenome() = delete;

    AbstractGenome(size_t _size) :size_(_size) {}

    virtual ~AbstractGenome() {}
    
    virtual size_t size() {return size_;}
    
    virtual void resize(size_t new_size) {size_ = new_size;};

    virtual std::byte* data() {
        std::cout << "data has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual AbstractGenome* clone() {
        std::cout << "mutate has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual void mutate() {
        std::cout << "mutate has not been written for this genome class" << std::endl;
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
};

namespace GN {





    template <class T>
    auto genomeRead(AbstractGenome* genome, int index) -> T& {
        return reinterpret_cast<T*>(genome->data() + index)[0];
    }

    template <class T>
    auto genomeWrite(AbstractGenome* genome, int index, T value) -> void {
        reinterpret_cast<T*>(genome->data() + index)[0] = value;
    }






    // python-like named arguments
    struct GenomeReadParams {
        AbstractGenome* genome;
        int index;
    };
    template <class T>
    auto genomeRead(const GenomeReadParams& params) -> T& {
        return genomeRead<T>(params.genome, params.index);
    }

    // python-like named arguments
    template <class T>
    struct GenomeWriteParams {
        AbstractGenome* genome;
        int index;
        T value;
    };
    template <class T>
    auto genomeWrite(const GenomeWriteParams<T>& params) -> void {
        genomeWrite<T>(params.genome, params.index, params.value);
    }
}