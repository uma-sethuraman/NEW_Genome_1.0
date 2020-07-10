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
        size_t size = 50;
        AbstractGenome* genome = new GenomeLite(size);

        for (size_t i(0); i < size; i++)
        {
            *(genome->data(i)) = (Byte)(i*2);
        }

        std::vector< Byte > segment(size, (Byte)10);
        genome->overwrite(0, segment);

        std::cout << "Test Constructor " << size << ": " BOLDGREEN << "PASSED" << RESET << std::endl;

        // test size 5000
        size = 50;
        AbstractGenome* genome2 = genome->clone();

        std::vector< Byte > segment2(20, (Byte)0);
        genome2->overwrite(10, segment2);

        genome2->show();
        genome->show();



        std::cout << "Test Constructor: " << size << BOLDGREEN << "PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

}