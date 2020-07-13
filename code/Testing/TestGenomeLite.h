/**
 * \file TestGenomeLite.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing genomeLite functions
 **/

#pragma once
#include <cassert>


#include "../AbstractGenome.h"
#include "../TestGenome.h"
#include "../GenomeLite/GenomeLite.h"

typedef std::byte Byte; ///< Byte for easy viewing

#define RESET   "\033[0m"
#define CYAN    "\033[36m"      /* Cyan */

#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */

/** handler testing functions **/
namespace TestGenomeLite
{
    void TestConstructor()
    {
        // test size 100
        size_t size = 5000;
        AbstractGenome* genome = new GenomeLite(size);
        genome->show();

        for (size_t i(3000); i < size; i++)
        {
            *(genome->data(i)) = (Byte)(i*2);
            std::cout << i << "\t" << (int)*(genome->data(i)) << std::endl;
        }

        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test clone
        AbstractGenome* genome2 = genome->clone();


        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

    void TestOverwrite()
    {
        // test size 100
        size_t size = 5000;
        AbstractGenome* genome = new GenomeLite(size);
        genome->show();
        
        // genome->overwrite(0, std::vector< Byte >(size, (Byte)10));

        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 5000
        size = 50;
        AbstractGenome* genome2 = genome->clone();
        // genome2->show();

        // genome2->overwrite(0, std::vector< Byte >(10, (Byte)0));

        // genome->show();
        // std::cout << "\t" BOLDBLUE "\tcopy" << std::endl; 
        // genome2->show();



        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

    void TestInsert()
    {
        // test size 100
        size_t size = 100;
        AbstractGenome* genome = new GenomeLite(size);
    
        // genome->show(); 

        std::cout << "Test Insert " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 5000
        AbstractGenome* genome2 = genome->clone();


        for (size_t i(1); i < 10; ++i)
        {
            genome2->insert(genome2->size(), std::vector< Byte >(2, (Byte)i));
            // std::cout << BOLDCYAN <<  i << "\t"<<  genome2->size()  << RESET << std::endl;
            assert(genome2->size() == size+(i*2));
            genome2->show();
        }

        genome2->show();


        std::cout << "Test Insert " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

    void TestDelete()
    {
        // test size 100
        size_t size = 50;
        AbstractGenome* genome = new GenomeLite(size);

        genome->overwrite(0, std::vector< Byte >(size, (Byte)0));


        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 5000
        size = 50;
        AbstractGenome* genome2 = genome->clone();

        genome2->insert(10, std::vector< Byte >(10, (Byte)5));

        genome2->show();
        std::cout << BOLDBLUE << std::endl;
        genome2->overwrite(15, std::vector< Byte >(10, (Byte)6));
        genome2->remove(0, 20);
        

        genome2->show();

        std::cout << RESET << genome2->size() << std::endl;


        std::cout << "Test Delete " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
        delete genome2;
    }

     void TestResize()
    {
        // test size 100
        size_t size = 1000;
        AbstractGenome* genome = new GenomeLite(size);

        genome->resize(5000);
        assert(genome->size() == 5000);
        genome->insert(2000, std::vector< Byte >(10, (Byte)5));
        assert(genome->size() == 5010);

        genome->resize(100);
        assert(genome->size() == 100);


        std::cout << "Test Resize " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        delete genome;
    }

}