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
        // genome->show();

        for (size_t i(0); i < size; i++)
        {
            *(genome->data(i)) = (Byte)(i*2);
            // std::cout << i << "\t" << (int)*(genome->data(i)) << std::endl;
        }

        
        genome->overwrite(0, std::vector< Byte >(size, (Byte)10));

        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 5000
        size = 50;
        AbstractGenome* genome2 = genome->clone();

        genome2->overwrite(0, std::vector< Byte >(10, (Byte)0));

        genome->show();
        std::cout << "\t" BOLDBLUE "\tcopy" << std::endl; 
        genome2->show();



        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

    void TestInsert()
    {
        // test size 100
        size_t size = 50;
        AbstractGenome* genome = new GenomeLite(size);
        // genome->show();
        
        genome->overwrite(0, std::vector< Byte >(size, (Byte)10));
        genome->insert(0, std::vector< Byte >(5, (Byte)0));

        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 5000
        size = 50;
        AbstractGenome* genome2 = genome->clone();

        genome2->insert(0, std::vector< Byte >(5, (Byte)0));


        genome->show();
        std::cout << "\t" BOLDBLUE "\tcopy" << std::endl; 
        genome2->show();



        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

    void TestDelete()
    {
        // test size 100
        size_t size = 50;
        AbstractGenome* genome = new GenomeLite(size);
        // genome->show();

        for (size_t i(0); i < size; i++)
        {
            *(genome->data(i)) = (Byte)(i);
            // std::cout << i << "\t" << (int)*(genome->data(i)) << std::endl;
        }

        
        genome->remove(10, 20);
        genome->show();

        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 5000
        size = 50;
        AbstractGenome* genome2 = genome->clone();

        // genome2->insert(0, std::vector< Byte >(5, (Byte)0));


        // genome->show();
        // std::cout << "\t" BOLDBLUE "\tcopy" << std::endl; 
        // genome2->show();



        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

}