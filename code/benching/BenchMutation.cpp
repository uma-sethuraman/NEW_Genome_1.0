#include "../experimentation/AbstractGenome.h"
#include "../experimentation/AbstractHandler.h"
#include "../experimentation/GenomeLite/Genome.h"
#include "../experimentation/GenomeLite/GenomeLite.h"
#include "../experimentation/SimpleGenome/SimpleGenome.h"


#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch.hpp"


TEST_CASE("Insertion Mutation", "[benchmark]") 
{
    // initializing genomes 
    size_t size = 1000000;
    AbstractGenome* genome = new Genome(size);
    AbstractGenome* simpleGenome = new SimpleGenome(size);
    for (size_t i(0); i < size; i++) { *(genome->data()+i) = (Byte)i; }

    AbstractGenome* genomeLite = new GenomeLite(genome);


    size_t numMutations = 100000; // 10,000
    // BENCHMARK("Test Simple Genome sz 1000000 del 100000 ins 100000") 
    // {
    //     Byte mutation[1] = {0x1};

    //     for (size_t i(numMutations); i--; )
    //     {
    //         simpleGenome->InsertMutation(rand()%(size), mutation, sizeof(mutation));
    //         simpleGenome->DeleteMutation(rand()%(size));
    //     }
    // };

    BENCHMARK("Test GenomeLite") 
    {
        Byte mutation[1] = {0x1};

        for (size_t i(numMutations); i--; )
        {
            genomeLite->InsertMutation(rand()%(size), mutation, sizeof(mutation));
            genomeLite->DeleteMutation(rand()%(size));
        }
    };

    delete genomeLite;
    delete genome;
    delete simpleGenome;
}