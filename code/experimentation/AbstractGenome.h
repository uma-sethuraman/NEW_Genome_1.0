#pragma once
#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <cstddef>
#include <vector>

#include "AbstractHandler.h"

typedef char Byte; // c++17 Byte doesn't always work

class AbstractGenome {
protected:
    size_t size_; // size should be size of memory when 

public:
    struct GeneDefinition {
        std::string name;
        std::vector<Byte> startCodon;
        size_t geneSize; // size in bytes
    };

    AbstractGenome() = delete;

    AbstractGenome(size_t _size) :size_(_size) {}

    virtual AbstractGenome* Clone() {
        std::cout << "Clone has not been written for this genome class" << std::endl;
        exit(1);
    }

    virtual ~AbstractGenome() {}
    
    virtual size_t size() {return size_;}
    
    virtual void resize(size_t new_size) {size_ = new_size;};

    virtual Byte* data() {
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

    virtual AbstractHandler* NewHandler() {
        std::cout << "Handler has not been created for this class" << std::endl;
        exit(1);
    }

      /** Makes a delete mutation at current index **/
    virtual void DeleteMutation(size_t index)
    {
        std::cout << "DeleteMutation has not been written for this Handler class" << std::endl;
        exit(1);
    }


    /** Makes a point mutation at current index
     * \param mutation point mutation to make**/
    virtual void PointMutation(size_t index, Byte* mutation, size_t size)
    {
        std::cout << "PointMutation has not been written for this Handler class" << std::endl;
        exit(1);
    }

    /** Makes a point mutation at current index
     * \param mutation point mutation to make**/

    virtual void InsertMutation(size_t index, Byte* mutation, size_t size)
    {
        std::cout << "InsertMutation has not been written for this Handler class" << std::endl;
        exit(1);
    }

    virtual void Print() {}
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