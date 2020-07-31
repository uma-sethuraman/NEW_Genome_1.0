/**
 * \file InvalidMutationTests.h
 * \author Uma Sethuraman
 **/

#include "AbstractGenome.h"
#include <string>
#include <cstddef>
#include <vector>

/* These tests test the out of bounds checks in the mutation methods.
   Make sure to run each test by itself, not all three together, 
   since each test should result in the program exiting.

   TO RUN THESE TESTS ON ANY GENOME:
    - in the main.cpp file, include "InvalidMutationTests.h"
    - in the main method, call InvalidMutationTests::invalidNAMETest<genomeName>(debug),
      where NAME is the name of the mutation (Overwrite, Remove, Insert),
      genomeName is the name of the genome class you want to test, and you can pass
      in true or false for debug based on if you want to see the debug output 
    - users can pass in true for debug and manually set what they would
      like to see as the debug output in the tests */

namespace InvalidMutationTests {

    /* overwriting at position past end of genome,
    should exit with error message, 
    should be called separate from other tests
    since it will exit immediately */
    template <class genomeName>
    void invalidOverwriteTest(bool debug) {
        AbstractGenome* genome = new genomeName(100);

        // trying to overwrite at position 110 in size 100 genome
        genome->overwrite(110, std::vector<std::byte>({(std::byte)10}));
    }

    /* removing past end of genome,
    should exit with error message,
    should be called separate from other tests
    since it will exit immediately */
    template <class genomeName>
    void invalidRemoveTest(bool debug) {
        AbstractGenome* genome = new genomeName(100);

        // removing 5 at position 100 will try to remove past end of genome
        genome->remove(100, 5);
    }

    /* inserting starting at position past end of genome,
    should exit with error message,
    should be called separate from other tests
    since it will exit immediately */
    template <class genomeName>
    void invalidInsertTest(bool debug) {
        AbstractGenome* genome = new genomeName(100);

        // trying to insert at position 110 in size 100 genome
        genome->insert(110, std::vector<std::byte>({(std::byte)10}));
    }
}