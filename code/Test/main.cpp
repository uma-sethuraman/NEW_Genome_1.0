// stl library
#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <bitset>
#include <random>
#include <map>
#include <cstddef>

// testing
#include "UmaChangelogTests.h"
#include "VictoriaTests.h"

// genomes
#include "AbstractGenome.h"
#include "TestGenome.h"
#include "GenomeLite.h"
#include "UmaGenome.h"

// misc
#include "utilities.h"


/** main function for running tests **/
int main() 
{
    /**     Uma's Testing Suite         **/
    /* pass in 0 to just see pass/fail results of all tests
       pass in 1 to see pass/fail results AND debug output of all tests
       pass in <GenomeName> based on which genome class you want to test */
    runUmaChangelogTests<UmaGenome>(0);

    /**     Victoria's Testing Suite         **/
    // pass in 0 to just see pass/fail results of all tests
    // pass in 1 to see pass/fail results AND debug output of all tests
    // pass in <GenomeName> based on which genome class you want to test
     VictoriaTests::TestAll<UmaGenome>(0);

    /* The tests below test the out of bounds checks in the mutation methods.
       Run each test by itself, not all three together, since each test should exit. */
    //invalidOverwriteTest<UmaGenome>(0);
    //invalidInsertTest<UmaGenome>(0);
    //invalidRemoveTest<UmaGenome>(0);

    return(0);
}