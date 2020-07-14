/**
 * \file VictoriaTests.h
 * \author Victoria Cao
 * 
 * \brief namespace for testing genome functions
 **/

#pragma once
#include <cassert>
#include <iostream>
#include <string>

#include "../AbstractGenome.h"
#include "../GenomeLite/GenomeLite.h"

typedef std::byte Byte; // c++17 Byte doesn't always work

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */


/** handler testing functions **/
namespace VictoriaTests
{

    /**
     * Tests cloning functions for making population
     **/
    template <class genomeName>
    void TestConstructor(bool debug)
    {
        std::cout << BOLDWHITE "Running " BOLDMAGENTA "CONSTRUCTOR TESTS " BOLDWHITE " for " BOLDMAGENTA << typeid(genomeName).name() << RESET << std::endl;   

        // test size 50
        size_t size = 50;
        AbstractGenome* genome = new genomeName(size);

        if (debug)
            genome->show();

        assert(genome->size() == size);
        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 500
        size = 500;

        AbstractGenome* temp = new genomeName(size);
        delete genome;
        genome = temp;

        if (debug)
            genome->show();

        assert(genome->size() == size);
        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        // test size 5000
        size = 5000;

        temp = new genomeName(size);
        delete genome;
        genome = temp;

        if (debug)
            genome->show();

        assert(genome->size() == size);
        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;

        // test size 50000
        size = 50000;

        temp = new genomeName(size);
        delete genome;
        genome = temp;

        if (debug)
            genome->show();

        assert(genome->size() == size);
        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        // test size 500000
        size = 500000;

        temp = new genomeName(size);
        delete genome;
        genome = temp;

        if (debug)
            genome->show();

        assert(genome->size() == size);
        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;



        // end of tests delete genome
        delete genome;
    }


    /**
     * Tests overwriting the whole genome
     **/
    template <class genomeName>
    void TestOverwriteAll(bool debug)
    {
        std::cout << BOLDWHITE "Running " BOLDMAGENTA "OVERWRITE ALL TESTS " BOLDWHITE " for " BOLDMAGENTA << typeid(genomeName).name() << RESET << std::endl;   

        // test size 50
        size_t size = 50;
        AbstractGenome* genome = new genomeName(size);

        genome->overwrite(0, std::vector<Byte>(size, (Byte)10));

        if (debug)
            genome->show();

        // assertions
        for (size_t i(0); i < size; i++)
        {
            assert(GN::genomeRead<Byte>(genome, i) == (Byte)10);
        }
        assert(genome->size() == size);

        std::cout << "Test Overwrite " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        // test size 500
        size = 500;

        AbstractGenome* temp = new genomeName(size);
        delete genome;
        genome = temp;

        genome->overwrite(0, std::vector<Byte>(size, (Byte)10));

        if (debug)
            genome->show();

        // assertions
        for (size_t i(0); i < size; i++)
        {
            assert(GN::genomeRead<Byte>(genome, i) == (Byte)10);
        }
        assert(genome->size() == size);


        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        // test size 5000
        size = 5000;

        temp = new genomeName(size);
        delete genome;
        genome = temp;

        genome->overwrite(0, std::vector<Byte>(size, (Byte)10));

        if (debug)
            genome->show();

        // assertions
        for (size_t i(0); i < size; i++)
        {
            assert(GN::genomeRead<Byte>(genome, i) == (Byte)10);
        }
        assert(genome->size() == size);


        std::cout << "Test Constructor " << size << ": " BOLDGREEN " PASSED" << RESET << std::endl;


        // delete genome once done with tests
        delete genome;

    }

    /**
     * Tests all of the overwrite functions
     **/
    template <class genomeName>
    void TestOverwrite(bool debug)
    {
        TestOverwriteAll<genomeName>(debug);
    }



    /**
     * Tests cloning functions for making population
     **/
    template <class genomeName>
    void TestInsert(bool debug)
    {

    }


    /**
     * Tests Delete function
     **/
    template <class genomeName>
    void TestDelete(bool debug)
    {

    }


    /**
     * Tests cloning functions for making population
     **/
    template <class genomeName>
    void TestClone(bool debug)
    {

    }



    /** 
     * Tests all of the previous tests
     **/
    template <class genomeName>
    void TestAll(bool debug)
    {
        TestConstructor<genomeName>(debug);
        TestOverwrite<genomeName>(debug);
        TestInsert<genomeName>(debug);
        TestDelete<genomeName>(debug);
        TestClone<genomeName>(debug);
    }

}