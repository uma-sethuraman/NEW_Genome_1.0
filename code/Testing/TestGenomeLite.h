/**
 * \file TestGenomeLite.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing genomeLite functions
 **/

#pragma once
#include <cassert>


#include "../Genome/AbstractGenome.h"
#include "../Genome/GenomeLite/GenomeLite.h"

typedef std::byte Byte; ///< Byte for easy viewing

#define RESET   "\033[0m"
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */

/** handler testing functions **/
namespace TestGenomeLite
{
    void TestConstructor()
    {
        size_t size = 100;
        AbstractGenome* genome = new GenomeLite(size);

        // genome->show();

        // for (size_t i(0); i < 40; i++)
        // {
        //     *genome->data(i) = (Byte)10;
        //     std::cout << i << std::endl;
        //     genome->show();
            
        // }
        // std::vector< Byte > segment(size, (Byte)size);
        // genome->overwrite(0, segment);

        // genome->show();


        delete genome;

        std::cout << "Test Constructor: " << BOLDGREEN << "PASSED" << RESET << std::endl;
    }

}