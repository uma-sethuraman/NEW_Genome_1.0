/**
 * \file TestGenomeLite.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing genomeLite functions
 **/

#pragma once
#include <cassert>


#include "../AbstractGenome.h"
#include "../GenomeLite/GenomeLite.h"

typedef std::byte Byte; ///< Byte for easy viewing

#define RESET   "\033[0m"
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */

/** handler testing functions **/
namespace TestGenomeLite
{
    void TestConstructor()
    {
        // test size 100
        size_t size = 100;
        // AbstractGenome* genome = new GenomeLite(size);

        // std::vector< Byte > segment(size, (Byte)10);
        // genome->overwrite(0, segment);

        // delete genome;

        // std::cout << "Test Constructor " << size << ": " BOLDGREEN << "PASSED" << RESET << std::endl;

        // test size 5000
        size = 5000;
        AbstractGenome* genome2 = new GenomeLite(size);

        genome2->show();

        std::vector< Byte > segment2(size, (Byte)10);
        genome2->overwrite(0, segment2);

        genome2->show();

        delete genome2;

        std::cout << "Test Constructor: " << size << BOLDGREEN << "PASSED" << RESET << std::endl;
    }

}