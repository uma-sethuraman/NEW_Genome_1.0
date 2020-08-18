#include "Genomes/AbstractGenome.h"
#include <string>
#include <cstddef>
#include <vector>

/* TO RUN THESE TESTS ON ANY GENOME:
   - in the main.cpp file, include "UmaChangelogTests.h"
   - in the main method, call runUmaChangelogTests<genomeName>(debug),
     where genomeName is the name of the genome class you want to test
     and you can pass in true or false for debug based on if you want
     to see the debug output 
   - users can manually set and change the debug output in any function
     if they do not want the default debug output */

/* checks if test passes or fails by 
   comparing genome passed in to the answer vector */
void printResults(std::vector<std::byte>& answer, AbstractGenome* gen, std::string testName) {
    bool failed = false;
    if (gen->size() != answer.size())
        failed = true;
    else {
        for(int i = 0; i < answer.size(); i++) {
            if(*(gen->data(i,1)) != answer[i]){
                failed = true;
            }
        }
    }
    
    std::cout << testName;
    if(failed)
        std::cout << "failed" << std::endl;
    else
        std::cout << "passed" << std::endl;
}

// 3 point mutations on size 10 genome
template <class genomeName>
void pointTest1(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 1's value to 4
    genome->overwrite(1, std::vector<std::byte>({(std::byte)(4)}));
    
    // point mutation - change position 7's value to  2
    genome->overwrite(7, std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 3's value to 4
    genome->overwrite(3, std::vector<std::byte>({(std::byte)(4)}));
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 4 2 0 1 2 4 3 1 0
     after mutation 2: 4 4 2 0 1 2 4 2 1 0
     after mutation 3: 4 4 2 4 1 2 4 2 1 0
     current genome: 4 4 2 4 1 2 4 2 1 0 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 4 2 4 1 2 4 2 1 0
    std::vector<std::byte> answer{(std::byte)4,(std::byte)4,(std::byte)2,
        (std::byte)4,(std::byte)1,(std::byte)2,
        (std::byte)4,(std::byte)2,(std::byte)1,
        (std::byte)0};
    
    printResults(answer, genome, "Point Test 1: ");
    
    delete genome;
}

// multiple point mutations on size 100 genome
template <class genomeName>
void pointTest2(bool debug) {
    
    // original genome of size 100: every value equals its index
    AbstractGenome* genome = new genomeName(100);
    for (int i = 0; i < 100; i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 10's value to 65
    genome->overwrite(10, std::vector<std::byte>({(std::byte)(65)}));
    
    // point mutation - change position 20's value to 75
    genome->overwrite(20, std::vector<std::byte>({(std::byte)(75)}));
    
    // point mutation - change position 30's value to 85
    genome->overwrite(30, std::vector<std::byte>({(std::byte)(85)}));
    
    // point mutation - change position 40's value to 95
    genome->overwrite(40, std::vector<std::byte>({(std::byte)(95)}));
    
    // point mutation - change position 50's value to 105
    genome->overwrite(50, std::vector<std::byte>({(std::byte)(105)}));
    
    // point mutation - change position 60's value to 205
    genome->overwrite(60, std::vector<std::byte>({(std::byte)(205)}));
    
    // point mutation - change position 70's value to 15
    genome->overwrite(70, std::vector<std::byte>({(std::byte)(15)}));
    
    // point mutation - change position 80's value to 20
    genome->overwrite(80, std::vector<std::byte>({(std::byte)(20)}));
    
    // point mutation - change position 90's value to 45
    genome->overwrite(90, std::vector<std::byte>({(std::byte)(45)}));
    
    // point mutation - change position 99's value to 70
    genome->overwrite(99, std::vector<std::byte>({(std::byte)(70)}));
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    /* correct answer: every value equals its index
     except for the 10 values in the if statements below */
    std::vector<std::byte> answer(genome->size());
    for (int index = 0; index < answer.size(); index++) {
        answer[index] = (std::byte)index;
        if (index == 10) answer[index] = (std::byte)65;
        if (index == 20) answer[index] = (std::byte)75;
        if (index == 30) answer[index] = (std::byte)85;
        if (index == 40) answer[index] = (std::byte)95;
        if (index == 50) answer[index] = (std::byte)105;
        if (index == 60) answer[index] = (std::byte)205;
        if (index == 70) answer[index] = (std::byte)15;
        if (index == 80) answer[index] = (std::byte)20;
        if (index == 90) answer[index] = (std::byte)45;
        if (index == 99) answer[index] = (std::byte)70;
    }
    
    printResults(answer, genome, "Point Test 2: ");
    
    delete genome;
}

// copy mutation on size 50 genome
template <class genomeName>
void copyTest1(bool debug) {
    AbstractGenome* genome = new genomeName(50);
    
    // original genome: value at every index initially equals the index itself
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    /* copy mutation - replace the first 10 values with the last 10 values,
     first 10 values should become: 40 41 42 43 44 45 46 47 48 49 */
    std::vector<std::byte> copyVals(10);
    int genIndex = genome->size()-copyVals.size();
    for (int copyPos = 0; copyPos < copyVals.size(); copyPos++){
        copyVals[copyPos] = GN::genomeRead<std::byte>(genome, genIndex);
        genIndex++;
    }
    genome->overwrite(0, copyVals);
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    /* correct answer: value at every index equals the index itself except the
     first 10 indices which have these values: 40 41 42 43 44 45 46 47 48 49 */
    std::vector<std::byte> answer(50);
    for (int aPos = 0; aPos < answer.size(); aPos++) {
        if (aPos >= 0 && aPos < copyVals.size())
            answer[aPos] = (std::byte)(aPos+40);
        else
            answer[aPos] = (std::byte)aPos;
    }
    
    printResults(answer, genome, "Copy Test 1: ");
    
    delete genome;
}

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

// deletion in the middle of size 10 genome
template <class genomeName>
void removeTest1(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove positions 5, 6, 7
    genome->remove(5,3);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 3 2 0 1 1 0
     current genome: 4 3 2 0 1 1 0 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 3 2 0 1 1 0
    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),
        (std::byte)(2),(std::byte)(0),
        (std::byte)(1),(std::byte)(1),
        (std::byte)(0)};
    
    printResults(answer, genome, "Remove Test 1: ");
    
    delete genome;
}

// point and remove mutations on size 10 genome
template <class genomeName>
void removeTest2(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 1's value to 1
    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    
    // point mutation - change position 3's value to 2
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 7's value to 2
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 9's value to 4
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
    
    // remove position 1
    genome->remove(1,1);
    
    // remove positions 3 and 4
    genome->remove(3,2);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 1 2 0 1 2 4 3 1 0
     after mutation 2: 4 1 2 2 1 2 4 3 1 0
     after mutation 3: 4 1 2 2 1 2 4 2 1 0
     after mutation 4: 4 1 2 2 1 2 4 2 1 4
     after mutation 5: 4 2 2 1 2 4 2 1 4
     after mutation 6: 4 2 2 4 2 1 4
     current genome: 4 2 2 4 2 1 4 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 2 2 4 2 1 4
    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),
        (std::byte)(2),(std::byte)(4),
        (std::byte)(2),(std::byte)(1),
        (std::byte)(4)};
    
    printResults(answer, genome, "Remove Test 2: ");
    
    delete genome;
}

// point and remove mutations on size 10 genome
template <class genomeName>
void removeTest3(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 1's value to 1
    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    
    // point mutation - change position 3's value to 2
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 7's value to 2
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 9's value to 4
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
    
    // remove positions 3 and 4
    genome->remove(3,2);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 1 2 0 1 2 4 3 1 0
     after mutation 2: 4 1 2 2 1 2 4 3 1 0
     after mutation 3: 4 1 2 2 1 2 4 2 1 0
     after mutation 4: 4 1 2 2 1 2 4 2 1 4
     after mutation 5: 4 1 2 2 4 2 1 4
     current genome: 4 1 2 2 4 2 1 4 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 1 2 2 4 2 1 4
    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(1),
        (std::byte)(2),(std::byte)(2),
        (std::byte)(4),(std::byte)(2),
        (std::byte)(1),(std::byte)(4)};
    
    printResults(answer, genome, "Remove Test 3: ");
    
    delete genome;
}

/* removing indices that were previously altered by point mutations
 in size 10 genome */
template <class genomeName>
void removeTest4(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 1's value to 1
    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    
    // point mutation - change position 3's value to 2
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 4's value to 3
    genome->overwrite(4,std::vector<std::byte>({(std::byte)(3)}));
    
    // point mutation - change position 7's value to 2
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    
    // point mutation - change position 9's value to 4
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
    
    // remove position 1
    genome->remove(1,1);
    
    // remove positions 3 and 4
    genome->remove(3,2);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 1 2 0 1 2 4 3 1 0
     after mutation 2: 4 1 2 2 1 2 4 3 1 0
     after mutation 3: 4 1 2 2 3 2 4 3 1 0
     after mutation 4: 4 1 2 2 3 2 4 2 1 0
     after mutation 5: 4 1 2 2 3 2 4 2 1 4
     after mutation 6: 4 2 2 3 2 4 2 1 4
     after mutation 7: 4 2 2 4 2 1 4
     current genome: 4 2 2 4 2 1 4 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 2 2 4 2 1 4
    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),
        (std::byte)(2),(std::byte)(4),
        (std::byte)(2),(std::byte)(1),
        (std::byte)(4)};
    
    printResults(answer, genome, "Remove Test 4: ");
    
    delete genome;
}

// point and remove mutations on size 10 genome
template <class genomeName>
void removeTest5(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 2's value to 3
    genome->overwrite(2,std::vector<std::byte>({(std::byte)(3)}));
    
    // point mutation - change position 5's value to 4
    genome->overwrite(5,std::vector<std::byte>({(std::byte)(4)}));
    
    // remove position 4
    genome->remove(4,1);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 3 3 0 1 2 4 3 1 0
     after mutation 2: 4 3 3 0 1 4 4 3 1 0
     after mutation 3: 4 3 3 0 4 4 3 1 0
     current genome: 4 3 3 0 4 4 3 1 0 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 3 3 0 4 4 3 1 0
    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,
        (std::byte)3,(std::byte)0,(std::byte)4,
        (std::byte)4,(std::byte)3,(std::byte)1,
        (std::byte)0};
    
    printResults(answer, genome, "Remove Test 5: ");
    
    delete genome;
}

// remove last element in size 10 genome
template <class genomeName>
void removeTest6(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove position 9 (last element)
    genome->remove(9,1);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 3 2 0 1 2 4 3 1
     current genome: 4 3 2 0 1 2 4 3 1 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 3 2 0 1 2 4 3 1
    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,
        (std::byte)2,(std::byte)0,
        (std::byte)1,(std::byte)2,
        (std::byte)4,(std::byte)3,
        (std::byte)1};
    
    printResults(answer, genome, "Remove Test 6: ");
    
    delete genome;
}

// remove first element in size 10 genome
template <class genomeName>
void removeTest7(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove position 0 (first element)
    genome->remove(0,1);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 3 2 0 1 2 4 3 1 0
     current genome: 3 2 0 1 2 4 3 1 0 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 3 2 0 1 2 4 3 1 0
    std::vector<std::byte> answer{(std::byte)3,(std::byte)2,
        (std::byte)0,(std::byte)1,
        (std::byte)2,(std::byte)4,
        (std::byte)3,(std::byte)1,
        (std::byte)0};
    
    printResults(answer, genome, "Remove Test 7: ");
    
    delete genome;
}

/* remove series of 4 elements starting at index 0
 in a size 10 genome */
template <class genomeName>
void removeTest8(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove positions 0, 1, 2, 3
    genome->remove(0,4);
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 1 2 4 3 1 0
     current genome: 1 2 4 3 1 0 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 1 2 4 3 1 0
    std::vector<std::byte> answer{(std::byte)1,(std::byte)2,
        (std::byte)4,(std::byte)3,
        (std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Remove Test 8: ");
    
    delete genome;
}

// alternating point and remove mutations on size 10 genome
template <class genomeName>
void removeTest9(bool debug) {
    
    // original genome: 4 3 2 0 1 2 4 3 1 0
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)1);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 2's value to 3
    genome->overwrite(2,std::vector<std::byte>({(std::byte)3}));
    
    // remove position 4 and 5
    genome->remove(4,2);
    
    // point mutation - change position 5's value to 4
    genome->overwrite(5,std::vector<std::byte>({(std::byte)4}));
    
    // remove positions 5, 6, 7
    genome->remove(5,3);
    
    // point mutation - change position 3's value to 4
    genome->overwrite(3,std::vector<std::byte>({(std::byte)4}));
    
    /* original genome: 4 3 2 0 1 2 4 3 1 0
     after mutation 1: 4 3 3 0 1 2 4 3 1 0
     after mutation 2: 4 3 3 0 4 3 1 0
     after mutation 3: 4 3 3 0 4 4 1 0
     after mutation 4: 4 3 3 0 4
     after mutation 5: 4 3 3 4 4
     current genome: 4 3 3 4 4 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 3 3 4 4
    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,
        (std::byte)3,(std::byte)4,
        (std::byte)4};
    
    printResults(answer, genome, "Remove Test 9: ");
    
    delete genome;
}

// alternating point and remove mutations on size 10 genome
template <class genomeName>
void removeTest10(bool debug) {
    
    // original genome: 40 34 21 9 0 2 11 67 48 12
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)40);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)34);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)21);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)9);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)67);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)48);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)12);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 4's value to 70
    genome->overwrite(4,std::vector<std::byte>({(std::byte)70}));
    
    // remove position 0
    genome->remove(0,1);
    
    // point mutation - change position 1's value to 49
    genome->overwrite(1,std::vector<std::byte>({(std::byte)49}));
    
    /* original genome: 40 34 21 9 0 2 11 67 48 12
     after mutation 1: 40 34 21 9 70 2 11 67 48 12
     after mutation 2: 34 21 9 70 2 11 67 48 12
     after mutation 3: 34 49 9 70 2 11 67 48 12
     current genome: 34 49 9 70 2 11 67 48 12 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 34 49 9 70 2 11 67 48 12
    std::vector<std::byte> answer{(std::byte)34,(std::byte)49,
        (std::byte)9,(std::byte)70,
        (std::byte)2,(std::byte)11,
        (std::byte)67,(std::byte)48,
        (std::byte)12};
    
    printResults(answer, genome, "Remove Test 10: ");
    
    delete genome;
}

/* removing a value that was just inserted
 in a size 10 genome */
template <class genomeName>
void removeTest11(bool debug) {
    
    // original genome: 11 22 33 44 55 66 77 88 99 111
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)66);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)77);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)88);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)99);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)111);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert value 212 at position 7
    genome->insert(7, std::vector<std::byte>({(std::byte)(212)}));
    
    // remove positions 5, 6, 7
    genome->remove(5, 3);
    
    /* original genome: 11 22 33 44 55 66 77 88 99 111
     after mutation 1: 11 22 33 44 55 66 77 212 88 99 111
     after mutation 2: 11 22 33 44 55 88 99 111
     current genome: 11 22 33 44 55 88 99 111 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // answer: 11 22 33 44 55 88 99 111
    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(22),
        (std::byte)(33),(std::byte)(44),
        (std::byte)(55), (std::byte)(88),
        (std::byte)(99), (std::byte)(111)};
    
    printResults(answer, genome, "Remove Test 11: ");
    
    delete genome;
}

/* removing multiple values that were just inserted
 in a size 10 genome */
template<class genomeName>
void removeTest12(bool debug) {
    
    // original genome: 11 22 33 44 55 66 77 88 99 111
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)66);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)77);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)88);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)99);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)111);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert {200, 250} starting at position 5
    genome->insert(5, std::vector<std::byte>({(std::byte)(200), (std::byte)(250)}));
    
    // remove positions 5,6,7,8
    genome->remove(5, 4);
    
    // insert {100, 110, 120, 130, 140} starting at position 6
    genome->insert(6, std::vector<std::byte>({(std::byte)(100), (std::byte)(110), (std::byte)(120), (std::byte)(130), (std::byte)(140)}));

    // remove positions 7 and 8
    genome->remove(7, 2);
    
    /* original genome: 11 22 33 44 55 66 77 88 99 111
     after mutation 1: 11 22 33 44 55 200 250 66 77 88 99 111
     after mutation 2: 11 22 33 44 55 88 99 111
     after mutation 3: 11 22 33 44 55 88 100 110 120 130 140 99 111
     after mutation 4: 11 22 33 44 55 88 100 130 140 99 111
     current genome: 11 22 33 44 55 88 100 130 140 99 111 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 22 33 44 55 88 100 130 140 99 111
    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(22),
        (std::byte)(33),(std::byte)(44),
        (std::byte)(55), (std::byte)(88),
        (std::byte)(100), (std::byte)(130),
        (std::byte)(140), (std::byte)(99),
        (std::byte)(111)};
    
    printResults(answer, genome, "Remove Test 12: ");
    
    delete genome;
}

// deletion of size 50 for size 200 genome
template <class genomeName>
void removeTest13(bool debug) {
    
    // original genome of size 200: all values equal to their indices
    AbstractGenome* genome = new genomeName(200);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove 50 values at position 1
    genome->remove(1, 50);
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    /* correct answer: genome of size 150
     with value at position 0 = 0 and
     values at position 1 to 149 = (50+position) */
    std::vector<std::byte> answer(150);
    answer[0] = (std::byte)0;
    for (int a = 1; a < answer.size(); a++) {
        answer[a] = (std::byte)(50+a);
    }
    
    printResults(answer, genome, "Remove Test 13: ");
    
    delete genome;
}

/* overlapping remove mutations (removing the same
 positions more than once) in a size 17 genome */
template <class genomeName>
void removeTest14(bool debug) {
    
    // original genome of size 17: all values equal to their indices
    AbstractGenome* genome = new genomeName(17);
    for (int i(0); i < genome->size(); i++) {
        GN::genomeWrite(genome, i, (std::byte)i);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove positions 0 and 1
    genome->remove(0, 2);
    
    // remove positions 3 and 4
    genome->remove(3, 2);
    
    // remove positions 7 and 8
    genome->remove(7, 2);
    
    /* remove positions 6,7,8.
     old 7 and 8 positions removed by previous mutation,
     this mutation should remove current 7 and 8 positions. */
    genome->remove(6, 3);
    
    /* original genome: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
     after mutation 1: 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
     after mutation 2: 2 3 4 7 8 9 10 11 12 13 14 15 16
     after mutation 3: 2 3 4 7 8 9 10 13 14 15 16
     after mutation 4: 2 3 4 7 8 9 15 16
     current genome: 2 3 4 7 8 9 15 16 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 2 3 4 7 8 9 15 16
    std::vector<std::byte> answer{(std::byte)2, (std::byte)3,
        (std::byte)4, (std::byte)7,
        (std::byte)8, (std::byte)9,
        (std::byte)15, (std::byte)16};
    
    printResults(answer, genome, "Remove Test 14: ");
    
    delete genome;
}

/* overlapping remove mutations (removing the same 
   positions more than once) in a size 17 genome */
template <class genomeName>
void removeTest14(bool debug) {

    // original genome of size 17: all values equal to their indices
    AbstractGenome* genome = new genomeName(17);
    for (int i(0); i < genome->size(); i++) {
        GN::genomeWrite(genome, i, (std::byte)i);
    }

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    // remove positions 0 and 1
    genome->remove(0, 2);

    // remove positions 3 and 4
    genome->remove(3, 2);

    // remove positions 7 and 8
    genome->remove(7, 2);

    /* remove positions 6,7,8.
       old 7 and 8 positions removed by previous mutation,
       this mutation should remove current 7 and 8 positions. */
    genome->remove(6, 3);

    /* original genome: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
       after mutation 1: 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
       after mutation 2: 2 3 4 7 8 9 10 11 12 13 14 15 16
       after mutation 3: 2 3 4 7 8 9 10 13 14 15 16
       after mutation 4: 2 3 4 7 8 9 15 16
       current genome: 2 3 4 7 8 9 15 16 */

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    // correct answer: 2 3 4 7 8 9 15 16
    std::vector<std::byte> answer{(std::byte)2, (std::byte)3,
                                  (std::byte)4, (std::byte)7,
                                  (std::byte)8, (std::byte)9,
                                  (std::byte)15, (std::byte)16};
    
    printResults(answer, genome, "Remove Test 14: ");

    delete genome;
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


// inserting single value into size 5 genome
template <class genomeName>
void insertTest1(bool debug) {
    
    // original genome: 11 22 33 44 55
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert value 99 at position 1
    std::vector<std::byte> vals{(std::byte)(99)};
    genome->insert(1,vals);
    
    /* original genome: 11 22 33 44 55
     after mutation 1: 11 99 22 33 44 55
     current genome: 11 99 22 33 44 55 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 99 22 33 44 55
    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,
        (std::byte)22,(std::byte)33,
        (std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 1: ");
    
    delete genome;
}

// inserting more than one value into size 5 genome
template <class genomeName>
void insertTest2(bool debug) {
    
    // original genome: 11 22 33 44 55
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert {99, 66} starting at position 1
    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(1,vals);
    
    /* original genome: 11 22 33 44 55
     after mutation 1: 11 99 66 22 33 44 55
     current genome: 11 99 66 22 33 44 55 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 99 66 22 33 44 55
    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,
        (std::byte)66,(std::byte)22,
        (std::byte)33,(std::byte)44,
        (std::byte)55};
    
    printResults(answer, genome, "Insert Test 2: ");
    
    delete genome;
}

// insert at beginning of size 5 genome
template <class genomeName>
void insertTest3(bool debug) {
    
    // original genome: 11 22 33 44 55
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert {99, 66} starting at position 0 (start of genome)
    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(0,vals);
    
    /* original genome: 11 22 33 44 55
     after mutation 1: 99 66 11 22 33 44 55
     current genome: 99 66 11 22 33 44 55 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 99 66 11 22 33 44 55
    std::vector<std::byte> answer{(std::byte)99,(std::byte)66,
        (std::byte)11,(std::byte)22,
        (std::byte)33,(std::byte)44,
        (std::byte)55};
    
    printResults(answer, genome, "Insert Test 3: ");
    
    delete genome;
}

// insert at end of size 5 genome
template <class genomeName>
void insertTest4(bool debug) {
    
    // original genome: 11 22 33 44 55
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert {99, 66} at position 5 (end of genome)
    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(5,vals);
    
    /* original genome: 11 22 33 44 55
     after mutation 1: 11 22 33 44 55 99 66
     current genome: 11 22 33 44 55 99 66 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 22 33 44 55 99 66
    std::vector<std::byte> answer{(std::byte)11,(std::byte)22,
        (std::byte)33,(std::byte)44,
        (std::byte)55,(std::byte)99,
        (std::byte)66};
    
    printResults(answer, genome, "Insert Test 4: ");
    
    delete genome;
}

// point and insert mutations on size 5 genome
template <class genomeName>
void insertTest5(bool debug) {
    
    // original genome: 4 3 2 0 1
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 1's value to 1
    genome->overwrite(1,std::vector<std::byte>({(std::byte)1}));
    
    // point mutation - change position 3's value to 2
    genome->overwrite(3,std::vector<std::byte>({(std::byte)2}));
    
    // point mutation - change position 4's value to 3
    genome->overwrite(4,std::vector<std::byte>({(std::byte)3}));
    
    // insert value 3 at position 1
    genome->insert(1,std::vector<std::byte>({(std::byte)(3)}));
    
    // insert {4,4} at position 3
    genome->insert(3,std::vector<std::byte>({(std::byte)(4), (std::byte)(4)}));
    
    /* original genome: 4 3 2 0 1
     after mutation 1: 4 1 2 0 1
     after mutation 2: 4 1 2 2 1
     after mutation 3: 4 1 2 2 3
     after mutation 4: 4 3 1 2 2 3
     after mutation 5: 4 3 1 4 4 2 2 3
     current genome: 4 3 1 4 4 2 2 3 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 3 1 4 4 2 2 3
    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,
        (std::byte)1,(std::byte)4,
        (std::byte)4,(std::byte)2,
        (std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "Insert Test 5: ");
    
    delete genome;
}

// insert, point, and copy mutations on size 10 genome
template <class genomeName>
void insertTest6(bool debug) {
    
    // original genome: 11 22 33 44 55 66 77 88 99 111
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)66);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)77);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)88);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)99);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)111);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert value 212 at position 7
    genome->insert(7, std::vector<std::byte>({(std::byte)(212)}));
    
    // point mutation - change position 5's value to 200
    genome->overwrite(5, std::vector<std::byte>({(std::byte)(200)}));
    
    // insert {15,16} starting at position 2
    genome->insert(2, std::vector<std::byte>({(std::byte)(15), (std::byte)(16)}));
    
    // copy mutation: replace positions 1,2,3 with the last 3 values in the genome
    std::vector<std::byte> copyVals(3);
    int genIndex = genome->size()-copyVals.size();
    for (int copyPos = 0; copyPos < copyVals.size(); copyPos++){
        copyVals[copyPos] = GN::genomeRead<std::byte>(genome, genIndex);
        genIndex++;
    }
    genome->overwrite(1, copyVals);
    
    /* original genome: 11 22 33 44 55 66 77 88 99 111
     after mutation 1: 11 22 33 44 55 66 77 212 88 99 111
     after mutation 2: 11 22 33 44 55 200 77 212 88 99 111
     after mutation 3: 11 22 15 16 33 44 55 200 77 212 88 99 111
     after mutation 4: 11 88 99 111 33 44 55 200 77 212 88 99 111
     current genome: 11 88 99 111 33 44 55 200 77 212 88 99 111 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 88 99 111 33 44 55 200 77 212 88 99 111
    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(88),
        (std::byte)(99),(std::byte)(111),
        (std::byte)(33),(std::byte)(44),
        (std::byte)(55), (std::byte)(200),
        (std::byte)(77), (std::byte)(212),
        (std::byte)(88), (std::byte)(99),
        (std::byte)(111)};
    
    printResults(answer, genome, "Insert Test 6: ");
    
    delete genome;
}

/* insert at a position that was previously removed
 in a size 10 genome */
template <class genomeName>
void insertTest7(bool debug) {
    
    // original genome: 0 1 2 3 4 5 6 7 8 9
    AbstractGenome* genome = new genomeName(10);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove positions 1 and 2
    genome->remove(1, 2);
    
    // insert {11,12} starting at position 1
    genome->insert(1, std::vector<std::byte>({(std::byte)11, (std::byte)12}));
    
    /* original genome: 0 1 2 3 4 5 6 7 8 9
     after mutation 1: 0 3 4 5 6 7 8 9
     after mutation 2: 0 11 12 3 4 5 6 7 8 9
     current genome: 0 11 12 3 4 5 6 7 8 9 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 0 11 12 3 4 5 6 7 8 9
    std::vector<std::byte> answer{(std::byte)(0),(std::byte)(11),
        (std::byte)(12),(std::byte)(3),
        (std::byte)(4),(std::byte)(5),
        (std::byte)(6), (std::byte)(7),
        (std::byte)(8), (std::byte)(9)};
    
    printResults(answer, genome, "Insert Test 7: ");
    
    delete genome;
}

/* insertions of multiple values at multiple positions that were
 previously removed in a size 15 genome */
template <class genomeName>
void insertTest8(bool debug) {
    
    // original genome: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
    AbstractGenome* genome = new genomeName(15);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // remove positions 0,1,2,3,4
    genome->remove(0, 5);
    
    // insert {17,18,19} starting at position 0
    genome->insert(0, std::vector<std::byte>({(std::byte)17, (std::byte)18, (std::byte)19}));
    
    // remove positions 2,3,4
    genome->remove(2, 3);
    
    // insert {27,28,29,30,31} at position 4
    genome->insert(4, std::vector<std::byte>({(std::byte)27, (std::byte)28, (std::byte)29, (std::byte)30, (std::byte)31}));
    
    /* original genome: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14
     after mutation 1: 5 6 7 8 9 10 11 12 13 14
     after mutation 2: 17 18 19 5 6 7 8 9 10 11 12 13 14
     after mutation 3: 17 18 7 8 9 10 11 12 13 14
     after mutation 4: 17 18 7 8 27 28 29 30 31 9 10 11 12 13 14
     current genome: 17 18 7 8 27 28 29 30 31 9 10 11 12 13 14 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 17 18 7 8 27 28 29 30 31 9 10 11 12 13 14
    std::vector<std::byte> answer{(std::byte)(17),(std::byte)(18),
        (std::byte)(7),(std::byte)(8),
        (std::byte)(27),(std::byte)(28),
        (std::byte)(29), (std::byte)(30),
        (std::byte)(31), (std::byte)(9),
        (std::byte)(10), (std::byte)(11),
        (std::byte)(12), (std::byte)(13),
        (std::byte)(14)};
    
    printResults(answer, genome, "Insert Test 8: ");
    
    delete genome;
}

/* insertion inception (insertion inside insertion inside insertion...)
 on a size 5 genome */
template <class genomeName>
void insertTest9(bool debug) {
    
    // original genome: 17 20 10 11 32
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)17);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)20);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)10);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)32);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert {81,82,83} starting at position 3
    genome->insert(3, std::vector<std::byte>({(std::byte)81,(std::byte)82, (std::byte)83}));
    
    // insert {81,82,83} starting at position 4
    genome->insert(4, std::vector<std::byte>({(std::byte)81,(std::byte)82, (std::byte)83}));
    
    // insert {71,72,73} starting at position 5
    genome->insert(5, std::vector<std::byte>({(std::byte)71,(std::byte)72, (std::byte)73}));
    
    // insert {61,62} starting at position 6
    genome->insert(6, std::vector<std::byte>({(std::byte)61,(std::byte)62}));
    
    // insert value 51 starting at position 7
    genome->insert(7, std::vector<std::byte>({(std::byte)51}));
    
    /* original genome: 17 20 10 11 32
     after mutation 1: 17 20 10 81 82 83 11 32
     after mutation 2: 17 20 10 81 81 82 83 82 83 11 32
     after mutation 3: 17 20 10 81 81 71 72 73 82 83 82 83 11 32
     after mutation 4: 17 20 10 81 81 71 61 62 72 73 82 83 82 83 11 32
     after mutation 5: 17 20 10 81 81 71 61 51 62 72 73 82 83 82 83 11 32
     current genome: 17 20 10 81 81 71 61 51 62 72 73 82 83 82 83 11 32 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 17 20 10 81 81 71 61 51 62 72 73 82 83 82 83 11 32
    std::vector<std::byte> answer{(std::byte)(17),(std::byte)(20),
        (std::byte)(10),(std::byte)(81),
        (std::byte)(81),(std::byte)(71),
        (std::byte)(61), (std::byte)(51),
        (std::byte)(62), (std::byte)(72),
        (std::byte)(73), (std::byte)(82),
        (std::byte)(83), (std::byte)(82),
        (std::byte)(83), (std::byte)(11),
        (std::byte)(32)};
    
    printResults(answer, genome, "Insert Test 9: ");
    
    delete genome;
}

// insertion of size 50 on size 100 genome
template <class genomeName>
void insertTest10(bool debug) {
    
    // original genome of size 100: all values are 50
    AbstractGenome* genome = new genomeName(100);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)50);
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert 50 values at position 1, all of which are 44
    std::vector<std::byte> vals(50, (std::byte)44);
    genome->insert(1, vals);
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    /* correct answer: genome of size 150
     with value at position 0 = 50,
     values at position 1 to 50 = 44,
     values at position 51 to 149 = 50 */
    std::vector<std::byte> answer(150, (std::byte)50);
    for (int a = 1; a <= 50; a++) {
        answer[a] = (std::byte)44;
    }
    
    printResults(answer, genome, "Insert Test 10: ");
    
    delete genome;
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

/* clones a size 100 genome and
 checks if clone data matches original data */
template <class genomeName>
void cloneTest1(bool debug) {
    
    // original genome of size 100: all values are equal to their indices
    AbstractGenome* genome = new genomeName(100);
    std::vector<std::byte> answer(100); // stores original genome data
    
    for(int i = 0; i < genome->size(); i++) {
        
        GN::genomeWrite(genome, i, (std::byte)i);
        answer[i] = (std::byte)i;
    }

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    AbstractGenome* clone = genome->clone();
    
    if (debug) {
        std::cout << "CLONED GENOME: ";
        clone->show();
        std::cout << std::endl;
    }
    
    printResults(answer, clone, "Clone Test 1: ");
    
    delete genome;
    delete clone;
}

/* cloning test on size 20 genome.
 clone original genome, mutate the clone,
 and check if clone's values are correct. */
template <class genomeName>
void cloneTest2(bool debug) {
    
    // original genome of size 20: all values are 0
    AbstractGenome* genome = new genomeName(20);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    AbstractGenome* clone = genome->clone();
    
    // Mutating the clone:
    
    // overwrite first 10 values to be 44
    clone->overwrite(0, std::vector<std::byte>(10, (std::byte)44));
    
    // remove positions 5 through 9
    clone->remove(5, 5);
    
    // insert {33, 33, 33, 33} at position 2
    clone->insert(2, std::vector<std::byte>(4, (std::byte)33));
    
    /* Original clone: 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
     After mutation 1: 44 44 44 44 44 44 44 44 44 44 0 0 0 0 0 0 0 0 0 0
     After mutation 2: 44 44 44 44 44 0 0 0 0 0 0 0 0 0 0
     After mutation 3: 44 44 33 33 33 33 44 44 44 0 0 0 0 0 0 0 0 0 0 */
    
    /* Cloning the clone, passed in forceCopy flag as true
     to tell the method to set the parent/exemplar genome
     to the current genome */
    AbstractGenome* finalClone = clone->clone(true);
    
    if (debug) {
        std::cout << "CLONED GENOME: ";
        clone->show();
        std::cout << "COPY OF CLONED GENOME: ";
        finalClone->show();
        std::cout << std::endl;
    }
    
    /* correct answer (what clone values should be):
     44 44 33 33 33 33 44 44 44 0 0 0 0 0 0 0 0 0 0 */
    std::vector<std::byte> answer(19, (std::byte)0);
    for (int i = 0; i <= 8; i++) {
        if(i >= 2 && i <= 5)
            answer[i] = (std::byte)33;
        else
            answer[i] = (std::byte)44;
    }
    
    // check if clone data is correct and matches finalClone data
    bool failed = false;
    if (clone->size() != answer.size())
        failed = true;
    else {
        for(int i = 0; i < answer.size(); i++) {
            if((*(clone->data(i, 1))) != answer[i]){
                failed = true;
            }
            if (*(clone->data(i,1)) != *(finalClone->data(i,1))) {
                failed = true;
            }
        }
    }
    
    std::cout << "Clone Test 2: ";
    if(failed)
        std::cout << "failed" << std::endl;
    else
        std::cout << "passed" << std::endl;
    
    delete genome;
    delete clone;
    delete finalClone;
}

/* cloning test on size 10 genome,
 10 generations of cloning and mutating. */
template <class genomeName>
void cloneTest3(bool debug) {
    
    // original genome of size 10: all values are 0
    AbstractGenome* genome = new genomeName(10);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    /* Clone genome, overwrite first 10 values to be 30.
     After overwrite: 30 30 30 30 30 30 30 30 30 30 */
    AbstractGenome* clone = genome->clone();
    clone->overwrite(0, std::vector<std::byte>(10, (std::byte)30));
    
    std::cout << "clone:" << std::endl;
    clone->show();
    
    /* Clone clone, overwrite position 2 to be 2, insert {3,4} at position 3.
     After overwrite: 30 30 2 30 30 30 30 30 30 30
     After insert: 30 30 2 3 4 30 30 30 30 30 30 30 */
    AbstractGenome* clone1 = clone->clone();
    clone1->overwrite(2, std::vector<std::byte>({(std::byte)2}));
    clone1->insert(3, std::vector<std::byte>({(std::byte)3, (std::byte)4}));
    
    std::cout << "clone1:" << std::endl;
    clone1->show();
    
    /* Clone clone1, remove positions 1 through 3.
     After remove: 30 4 30 30 30 30 30 30 30 */
    AbstractGenome* clone2 = clone1->clone();
    clone2->remove(1, 3);
    
    std::cout << "clone2:" << std::endl;
    clone2->show();
    
    /* Clone clone2, insert {40, 40, 40, 40} at position 4, insert 50 at position 5.
     After first insert:  30 4 30 30 40 40 40 40 30 30 30 30 30
     After second insert: 30 4 30 30 40 50 40 40 40 30 30 30 30 30 */
    AbstractGenome* clone3 = clone2->clone();
    clone3->insert(4, std::vector<std::byte>(4, (std::byte)40));
    clone3->insert(5, std::vector<std::byte>({(std::byte)50}));
    
    std::cout << "clone3:" << std::endl;
    clone3->show();
    
    /* Clone clone3, overwrite positions 0 to 3 with value 10.
     After overwrite: 10 10 10 10 40 50 40 40 40 30 30 30 30 30 */
    AbstractGenome* clone4 = clone3->clone();
    clone4->overwrite(0, std::vector<std::byte>(4, (std::byte)10));
    
    std::cout << "clone4:" << std::endl;
    clone4->show();
    
    /* Clone clone4, remove positions 0 and 1, remove positions 10 and 11.
     After first remove: 10 10 40 50 40 40 40 30 30 30 30 30
     After second remove: 10 10 40 50 40 40 40 30 30 30 */
    AbstractGenome* clone5 = clone4->clone();
    clone5->remove(0, 2);
    clone5->remove(10, 2);
    
    std::cout << "clone5:" << std::endl;
    clone5->show();
    
    /* Clone clone5, overwrite positions 7 and 8 with value 70.
     After overwrite: 10 10 40 50 40 40 40 70 70 30 */
    AbstractGenome* clone6 = clone5->clone();
    clone6->overwrite(7, std::vector<std::byte>(2, (std::byte)70));
    
    std::cout << "clone6:" << std::endl;
    clone6->show();
    
    /* Clone clone6, insert {20, 20, 20} at position 3.
     After insert: 20 20 20 10 10 40 50 40 40 40 70 70 30 */
    std::cout << "----- clone7 -----" << std::endl;
    std::cout << "clone6:" << std::endl;
    clone6->show();
    AbstractGenome* clone7 = clone6->clone();
    std::cout << "clone6:" << std::endl;
    clone6->show();
    std::cout << "clone7:" << std::endl;
    clone7->show();

    std::cout << "----- clone7 done -----" << std::endl;
    clone7->insert(0, std::vector<std::byte>(3, (std::byte)20));
    std::cout << "----- clone7 insert done -----" << std::endl;

    /* Clone clone7, remove positions 1 through 5.
     After remove: 20 50 40 40 40 70 70 30 */
    std::cout << "----- clone8 -----" << std::endl;
    AbstractGenome* clone8 = clone7->clone();
    std::cout << "----- clone8 done -----" << std::endl;
    clone8->remove(1, 5);
    std::cout << "----- clone8 remove done -----" << std::endl;

    
    /* Clone clone8, overwrite positions 5 through 7 with value 90.
     After overwrite: 20 50 40 40 40 90 90 90 */
    AbstractGenome* clone9 = clone8->clone();
    clone9->overwrite(5, std::vector<std::byte>(3, (std::byte)90));
    
    /* Clone clone9, passed in forceCopy flag as true
     to tell the method to set the parent/exemplar genome
     to the current genome.
     same values as clone9: 20 50 40 40 40 90 90 90 */
    AbstractGenome* finalClone = clone9->clone(true);
    
    if (debug) {
        std::cout << "LAST MUTATED CLONE: ";
        clone9->show();
        std::cout << "COPY OF LAST MUTATED CLONE: ";
        finalClone->show();
        std::cout << std::endl;
    }
    
    /* correct answer (clone9 and finalClone data):
     20 50 40 40 40 90 90 90 */
    std::vector<std::byte> answer{(std::byte)20, (std::byte)50,
        (std::byte)40, (std::byte)40,
        (std::byte)40, (std::byte)90,
        (std::byte)90, (std::byte)90};
    
    // check if clone9 data is correct and matches finalClone data
    bool failed = false;
    if (clone9->size() != answer.size())
        failed = true;
    else {
        for(int i = 0; i < answer.size(); i++) {
            if((*(clone9->data(i, 1))) != answer[i]){
                failed = true;
            }
            if (*(clone9->data(i,1)) != *(finalClone->data(i,1))) {
                failed = true;
            }
        }
    }
    
    std::cout << "Clone Test 3: ";
    if(failed)
        std::cout << "failed" << std::endl;
    else
        std::cout << "passed" << std::endl;
    
    // delete original genome and all clones
    delete genome;
    delete clone;
    delete clone1;
    delete clone2;
    delete clone3;
    delete clone4;
    delete clone5;
    delete clone6;
    delete clone7;
    delete clone8;
    delete clone9;
    delete finalClone;
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

/* clones a size 100 genome and
   checks if clone data matches original data */
template <class genomeName>
void cloneTest1(bool debug) {

    // original genome of size 100: all values are equal to their indices
    AbstractGenome* genome = new genomeName(100);
    std::vector<std::byte> answer(100); // stores original genome data
    for(int i = 0; i < genome->size(); i++) {
        GN::genomeWrite(genome, i, (std::byte)i);
        answer[i] = (std::byte)i;
    }

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    AbstractGenome* clone = genome->clone();

    if (debug) {
        std::cout << "CLONED GENOME: ";
        clone->show();
        std::cout << std::endl;
    }

    printResults(answer, clone, "Clone Test 1: ");

    delete genome;
    delete clone;
}

/* cloning test on size 20 genome.
   clone original genome, mutate the clone,
   and check if clone's values are correct. */
template <class genomeName>
void cloneTest2(bool debug) {

    // original genome of size 20: all values are 0
    AbstractGenome* genome = new genomeName(20);

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    AbstractGenome* clone = genome->clone();

    // Mutating the clone: 

    // overwrite first 10 values to be 44
    clone->overwrite(0, std::vector<std::byte>(10, (std::byte)44));

    // remove positions 5 through 9
    clone->remove(5, 5);

    // insert {33, 33, 33, 33} at position 2
    clone->insert(2, std::vector<std::byte>(4, (std::byte)33));

    /* Original clone: 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
       After mutation 1: 44 44 44 44 44 44 44 44 44 44 0 0 0 0 0 0 0 0 0 0
       After mutation 2: 44 44 44 44 44 0 0 0 0 0 0 0 0 0 0
       After mutation 3: 44 44 33 33 33 33 44 44 44 0 0 0 0 0 0 0 0 0 0 */

    /* Cloning the clone, passed in forceCopy flag as true
       to tell the method to set the parent/exemplar genome
       to the current genome */
    AbstractGenome* finalClone = clone->clone(true);

    if (debug) {
        std::cout << "CLONED GENOME: ";
        clone->show();
        std::cout << "COPY OF CLONED GENOME: ";
        finalClone->show();
        std::cout << std::endl;
    }

    /* correct answer (what clone values should be):
       44 44 33 33 33 33 44 44 44 0 0 0 0 0 0 0 0 0 0 */
    std::vector<std::byte> answer(19, (std::byte)0);
    for (int i = 0; i <= 8; i++) {
        if(i >= 2 && i <= 5)
            answer[i] = (std::byte)33;
        else
            answer[i] = (std::byte)44;
    }

    // check if clone data is correct and matches finalClone data
    bool failed = false;
    if (clone->size() != answer.size())
        failed = true;
    else {
        for(int i = 0; i < answer.size(); i++) {
            if((*(clone->data(i, 1))) != answer[i]){
                failed = true;
            }
            if (*(clone->data(i,1)) != *(finalClone->data(i,1))) {
                failed = true;
            }
        }
    }

    std::cout << "Clone Test 2: ";
    if(failed) 
        std::cout << "failed" << std::endl;
    else
        std::cout << "passed" << std::endl;
    
    delete genome;
    delete clone;
    delete finalClone;
}

/* cloning test on size 10 genome,
   10 generations of cloning and mutating. */
template <class genomeName>
void cloneTest3(bool debug) {

    // original genome of size 10: all values are 0
    AbstractGenome* genome = new genomeName(10);

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    /* Clone genome, overwrite first 10 values to be 30.
       After overwrite: 30 30 30 30 30 30 30 30 30 30 */
    AbstractGenome* clone = genome->clone();
    clone->overwrite(0, std::vector<std::byte>(10, (std::byte)30));

    /* Clone clone, overwrite position 2 to be 2, insert {3,4} at position 3.
       After overwrite: 30 30 2 30 30 30 30 30 30 30
       After insert: 30 30 2 3 4 30 30 30 30 30 30 30 */
    AbstractGenome* clone1 = clone->clone();
    clone1->overwrite(2, std::vector<std::byte>({(std::byte)2}));
    clone1->insert(3, std::vector<std::byte>({(std::byte)3, (std::byte)4}));

    /* Clone clone1, remove positions 1 through 3.
       After remove: 30 4 30 30 30 30 30 30 30 */
    AbstractGenome* clone2 = clone1->clone();
    clone2->remove(1, 3);

    /* Clone clone2, insert {40, 40, 40, 40} at position 4, insert 50 at position 5.
       After first insert:  30 4 30 30 40 40 40 40 30 30 30 30 30
       After second insert: 30 4 30 30 40 50 40 40 40 30 30 30 30 30 */
    AbstractGenome* clone3 = clone2->clone();
    clone3->insert(4, std::vector<std::byte>(4, (std::byte)40));
    clone3->insert(5, std::vector<std::byte>({(std::byte)50}));

    /* Clone clone3, overwrite positions 0 to 3 with value 10.
       After overwrite: 10 10 10 10 40 50 40 40 40 30 30 30 30 30 */
    AbstractGenome* clone4 = clone3->clone();
    clone4->overwrite(0, std::vector<std::byte>(4, (std::byte)10));

    /* Clone clone4, remove positions 0 and 1, remove positions 10 and 11.
       After first remove: 10 10 40 50 40 40 40 30 30 30 30 30
       After second remove: 10 10 40 50 40 40 40 30 30 30 */
    AbstractGenome* clone5 = clone4->clone();
    clone5->remove(0, 2);
    clone5->remove(10, 2);

    /* Clone clone5, overwrite positions 7 and 8 with value 70.
       After overwrite: 10 10 40 50 40 40 40 70 70 30 */
    AbstractGenome* clone6 = clone5->clone();
    clone6->overwrite(7, std::vector<std::byte>(2, (std::byte)70));

    /* Clone clone6, insert {20, 20, 20} at position 3.
       After insert: 20 20 20 10 10 40 50 40 40 40 70 70 30 */
    AbstractGenome* clone7 = clone6->clone();
    clone7->insert(0, std::vector<std::byte>(3, (std::byte)20));

    /* Clone clone7, remove positions 1 through 5.
       After remove: 20 50 40 40 40 70 70 30 */
    AbstractGenome* clone8 = clone7->clone();
    clone8->remove(1, 5);

    /* Clone clone8, overwrite positions 5 through 7 with value 90.
       After overwrite: 20 50 40 40 40 90 90 90 */
    AbstractGenome* clone9 = clone8->clone();
    clone9->overwrite(5, std::vector<std::byte>(3, (std::byte)90));

    /* Clone clone9, passed in forceCopy flag as true
       to tell the method to set the parent/exemplar genome
       to the current genome. 
       same values as clone9: 20 50 40 40 40 90 90 90 */
    AbstractGenome* finalClone = clone9->clone(true);

    if (debug) {
        std::cout << "LAST MUTATED CLONE: ";
        clone9->show();
        std::cout << "COPY OF LAST MUTATED CLONE: ";
        finalClone->show();
        std::cout << std::endl;
    }

    /* correct answer (clone9 and finalClone data):
       20 50 40 40 40 90 90 90 */
    std::vector<std::byte> answer{(std::byte)20, (std::byte)50, 
                                  (std::byte)40, (std::byte)40,
                                  (std::byte)40, (std::byte)90,
                                  (std::byte)90, (std::byte)90};

    // check if clone9 data is correct and matches finalClone data
    bool failed = false;
    if (clone9->size() != answer.size())
        failed = true;
    else {
        for(int i = 0; i < answer.size(); i++) {
            if((*(clone9->data(i, 1))) != answer[i]){
                failed = true;
            }
            if (*(clone9->data(i,1)) != *(finalClone->data(i,1))) {
                failed = true;
            }
        }
    }

    std::cout << "Clone Test 3: ";
    if(failed) 
        std::cout << "failed" << std::endl;
    else
        std::cout << "passed" << std::endl;
    
    // delete original genome and all clones
    delete genome;
    delete clone;
    delete clone1;
    delete clone2;
    delete clone3;
    delete clone4;
    delete clone5;
    delete clone6;
    delete clone7;
    delete clone8;
    delete clone9;
    delete finalClone;
}

// point, insert, and remove mutations on size 5 genome
template <class genomeName>
void allMutationsTest1(bool debug) {
    
    // original genome: 4 3 2 0 1
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)4);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)3);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)1);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // point mutation - change position 1's value to 1
    genome->overwrite(1,std::vector<std::byte>({(std::byte)1}));
    
    // point mutation - change position 3's value to 2
    genome->overwrite(3,std::vector<std::byte>({(std::byte)2}));
    
    // point mutation - change position 4's value to 3
    genome->overwrite(4,std::vector<std::byte>({(std::byte)3}));
    
    // insert value 3 at position 1
    genome->insert(1,std::vector<std::byte>({(std::byte)(3)}));
    
    // insert {4,4} at position 3
    genome->insert(3,std::vector<std::byte>({(std::byte)(4), (std::byte)(4)}));
    
    // point mutation - change position 4's value to 2
    genome->overwrite(4,std::vector<std::byte>({(std::byte)2}));
    
    // remove positions 5 and 6
    genome->remove(5,2);
    
    /* original genome: 4 3 2 0 1
     after mutation 1: 4 1 2 0 1
     after mutation 2: 4 1 2 2 1
     after mutation 3: 4 1 2 2 3
     after mutation 4: 4 3 1 2 2 3
     after mutation 5: 4 3 1 4 4 2 2 3
     after mutation 6: 4 3 1 4 2 2 2 3
     after mutation 7: 4 3 1 4 2 3
     current genome: 4 3 1 4 2 3 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 4 3 1 4 2 3
    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,
        (std::byte)1,(std::byte)4,
        (std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "All-Mutations Test 1: ");
    
    delete genome;
}

// point, insert, and remove mutations on size 10 genome
template <class genomeName>
void allMutationsTest2(bool debug) {
    
    // original genome: 40 34 21 9 0 2 11 67 48 12
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)40);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)34);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)21);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)9);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)0);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)2);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)67);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)48);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)12);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert {60,50,51} starting at position 0
    genome->insert(0,std::vector<std::byte>({(std::byte)(60), (std::byte)(50), (std::byte)(51)}));
    
    // point mutation - change position 4's value to 70
    genome->overwrite(4,std::vector<std::byte>({(std::byte)70}));
    
    // remove position 0
    genome->remove(0,1);
    
    // insert {62,52,42} starting at position 12
    genome->insert(12,std::vector<std::byte>({(std::byte)(62), (std::byte)(52), (std::byte)(42)}));
    
    // remove positions 12 and 13
    genome->remove(12,2);
    
    // insert value 99 at position 3
    genome->insert(3,std::vector<std::byte>({(std::byte)(99)}));
    
    // remove position 12
    genome->remove(12,1);
    
    // point mutation - change position 1's value to 49
    genome->overwrite(1,std::vector<std::byte>({(std::byte)49}));
    
    /* original genome: 40 34 21 9 0 2 11 67 48 12
     after mutation 1: 60 50 51 40 34 21 9 0 2 11 67 48 12
     after mutation 2: 60 50 51 40 70 21 9 0 2 11 67 48 12
     after mutation 3: 50 51 40 70 21 9 0 2 11 67 48 12
     after mutation 4: 50 51 40 70 21 9 0 2 11 67 48 12 62 52 42
     after mutation 5: 50 51 40 70 21 9 0 2 11 67 48 12 42
     after mutation 6: 50 51 40 99 70 21 9 0 2 11 67 48 12 42
     after mutation 7: 50 51 40 99 70 21 9 0 2 11 67 48 42
     after mutation 8: 50 49 40 99 70 21 9 0 2 11 67 48 42
     current genome: 50 49 40 99 70 21 9 0 2 11 67 48 42 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 50 49 40 99 70 21 9 0 2 11 67 48 42
    std::vector<std::byte> answer{(std::byte)50,(std::byte)49,
        (std::byte)40,(std::byte)99,
        (std::byte)70,(std::byte)21,
        (std::byte)9,(std::byte)0,
        (std::byte)2,(std::byte)11,
        (std::byte)67,(std::byte)48,
        (std::byte)42};
    
    printResults(answer, genome, "All-Mutations Test 2: ");
    
    delete genome;
}

// point, insert, and remove mutations on size 5 genome
template <class genomeName>
void allMutationsTest3(bool debug) {
    
    // original genome: 11 22 33 44 55
    AbstractGenome* genome = new genomeName(5);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert value 99 at position 1
    genome->insert(1, std::vector<std::byte>({(std::byte)(99)}));
    
    // remove positions 2 and 3
    genome->remove(2, 2);
    
    // point mutation - change position 2's value to 65
    genome->overwrite(2, std::vector<std::byte>({(std::byte)(65)}));
    
    // insert {88,77} starting at position 2
    genome->insert(2, std::vector<std::byte>({(std::byte)(88), (std::byte)(77)}));
    
    /* original genome: 11 22 33 44 55
     after mutation 1: 11 99 22 33 44 55
     after mutation 2: 11 99 44 55
     after mutation 3: 11 99 65 55
     after mutation 4: 11 99 88 77 65 55
     current genome: 11 99 88 77 65 55 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 99 88 77 65 55
    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(99),
        (std::byte)(88),(std::byte)(77),
        (std::byte)(65),(std::byte)(55)};
    
    printResults(answer, genome, "All-Mutations Test 3: ");
    
    delete genome;
}

// point, copy, remove, and insert mutations on size 10 genome
template <class genomeName>
void allMutationsTest4(bool debug) {
    
    // original genome: 11 22 33 44 55 66 77 88 99 111
    AbstractGenome* genome = new genomeName(10);
    GN::genomeWrite<std::byte>(genome, 0, (std::byte)11);
    GN::genomeWrite<std::byte>(genome, 1, (std::byte)22);
    GN::genomeWrite<std::byte>(genome, 2, (std::byte)33);
    GN::genomeWrite<std::byte>(genome, 3, (std::byte)44);
    GN::genomeWrite<std::byte>(genome, 4, (std::byte)55);
    GN::genomeWrite<std::byte>(genome, 5, (std::byte)66);
    GN::genomeWrite<std::byte>(genome, 6, (std::byte)77);
    GN::genomeWrite<std::byte>(genome, 7, (std::byte)88);
    GN::genomeWrite<std::byte>(genome, 8, (std::byte)99);
    GN::genomeWrite<std::byte>(genome, 9, (std::byte)111);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // insert value 212 at position 7
    genome->insert(7, std::vector<std::byte>({(std::byte)(212)}));
    
    // remove positions 5,6,7
    genome->remove(5, 3);
    
    // point mutation - change position 5's value to 200
    genome->overwrite(5, std::vector<std::byte>({(std::byte)(200)}));
    
    // insert {15,16} starting at position 2
    genome->insert(2, std::vector<std::byte>({(std::byte)(15), (std::byte)(16)}));
    
    // copy mutation: replace positions 1,2,3 with the last 3 values in the genome
    std::vector<std::byte> copyVals(3);
    int genIndex = genome->size()-copyVals.size();
    for (int copyPos = 0; copyPos < copyVals.size(); copyPos++){
        copyVals[copyPos] = GN::genomeRead<std::byte>(genome, genIndex);
        genIndex++;
    }
    genome->overwrite(1, copyVals);
    
    /* original genome: 11 22 33 44 55 66 77 88 99 111
     after mutation 1: 11 22 33 44 55 66 77 212 88 99 111
     after mutation 2: 11 22 33 44 55 88 99 111
     after mutation 3: 11 22 33 44 55 200 99 111
     after mutation 4: 11 22 15 16 33 44 55 200 99 111
     after mutation 5: 11 200 99 111 33 44 55 200 99 111
     current genome: 11 200 99 111 33 44 55 200 99 111 */
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }
    
    // correct answer: 11 200 99 111 33 44 55 200 99 111
    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(200),
        (std::byte)(99),(std::byte)(111),
        (std::byte)(33),(std::byte)(44),
        (std::byte)(55), (std::byte)(200),
        (std::byte)(99), (std::byte)(111)};
    
    printResults(answer, genome, "All-Mutations Test 4: ");
    
    delete genome;
}

/* multiple point mutations on size 100000 large genome, 
   initially rewrites all values in genome,
   debug doesn't print entire genome since it's very large,
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest1(bool debug) {
    
    // original genome of size 100000: every value equals its index (as a byte)
    AbstractGenome* genome = new genomeName(100000);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }
    
    // point mutation - change position 100's value to 65
    genome->overwrite(100, std::vector<std::byte>({(std::byte)(65)}));
    
    // point mutation - change position 200's value to 75
    genome->overwrite(200, std::vector<std::byte>({(std::byte)(75)}));
    
    // point mutation - change position 300's value to 85
    genome->overwrite(300, std::vector<std::byte>({(std::byte)(85)}));
    
    // point mutation - change position 4000's value to 95
    genome->overwrite(4000, std::vector<std::byte>({(std::byte)(95)}));
  
    // point mutation - change position 5000's value to 105
    genome->overwrite(5000, std::vector<std::byte>({(std::byte)(105)}));
        
    // point mutation - change position 6000's value to 205
    genome->overwrite(6000, std::vector<std::byte>({(std::byte)(205)}));
        
    // point mutation - change position 70000's value to 15
    genome->overwrite(70000, std::vector<std::byte>({(std::byte)(15)}));
    
    // point mutation - change position 80000's value to 20
    genome->overwrite(80000, std::vector<std::byte>({(std::byte)(20)}));
    
    // point mutation - change position 90000's value to 45
    genome->overwrite(90000, std::vector<std::byte>({(std::byte)(45)}));
    
    // point mutation - change position 99999's value to 70
    genome->overwrite(99999, std::vector<std::byte>({(std::byte)(70)}));
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    /* correct answer: genome of size 100000
     with every value = (std::byte)(its index)
     except the 10 values in the if statements below */
    std::vector<std::byte> answer(genome->size());
    for (int index = 0; index < answer.size(); index++) {
        answer[index] = (std::byte)index;
        if (index == 100) answer[index] = (std::byte)65;
        if (index == 200) answer[index] = (std::byte)75;
        if (index == 300) answer[index] = (std::byte)85;
        if (index == 4000) answer[index] = (std::byte)95;
        if (index == 5000) answer[index] = (std::byte)105;
        if (index == 6000) answer[index] = (std::byte)205;
        if (index == 70000) answer[index] = (std::byte)15;
        if (index == 80000) answer[index] = (std::byte)20;
        if (index == 90000) answer[index] = (std::byte)45;
        if (index == 99999) answer[index] = (std::byte)70;
    }
    
    printResults(answer, genome, "Stress Test 1: ");
    
    delete genome;
}

/* insertion of size 300 at beginning of size 100000 genome,
   initially rewrites all values in genome,
   debug doesn't print entire genome since it's very large,
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest2(bool debug) {
    
    // original genome of size 100000: all values are 17
    AbstractGenome* genome = new genomeName(100000);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)17);
    }
    
    // insert 300 values at position 0, all of which are 77
    std::vector<std::byte> vals(300, (std::byte)66);
    genome->insert(0, vals);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    /* correct answer: genome of size 100300
     with values at position 0 to 299 = 66
     and values at position 300 to 100299 = 17 */
    std::vector<std::byte> answer(100300, (std::byte)17);
    for (int a = 0; a < 300; a++) {
        answer[a] = (std::byte)66;
    }
    
    printResults(answer, genome, "Stress Test 2: ");
    
    delete genome;
}

/* deletion of size 300 at beginning of size 100000 genome,
   initially rewrites all values in genome,
   debug doesn't print entire genome since it's very large,
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest3(bool debug) {
    
    // original genome of size 100000: all values are 17
    AbstractGenome* genome = new genomeName(100000);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)17);
    }
    
    // remove 300 values at position 0
    genome->remove(0, 300);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    // correct answer: genome of size 99700 with all values equal to 17
    std::vector<std::byte> answer(99700, (std::byte)17);
    
    printResults(answer, genome, "Stress Test 3: ");
    
    delete genome;
}

/* point, copy, insert, and delete mutations on size 1 million genome,
   initially rewrites all values in genome, 
   debug doesn't print entire genome since it's very large, 
   user can modify test to print what they want when debug is set */
template<class genomeName>
void stressTest4(bool debug) {
    AbstractGenome* genome = new genomeName(1000000);
    
    // original genome: all 1 million values set to 10 initially
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)10);
    }
    
    // remove first 200 sites, size becomes 999800
    genome->remove(0, 200);
    
    // insert 300 sites of value 15 at position 0, size becomes 1000100
    std::vector<std::byte> insertionVals(300, (std::byte)15);
    genome->insert(0, insertionVals);
    
    // point mutation - change position 5's value to 30
    genome->overwrite(5, std::vector<std::byte>({(std::byte)30}));
    
    /* set positions 301 to 351 to value 15 by copy mutation,
     copy the values from position 200 to 249 into the positions 301 to 351 */
    std::vector<std::byte> copyVals(50);
    for (int copyPos = 200; copyPos < 250; copyPos++)
        copyVals[copyPos-200] = GN::genomeRead<std::byte>(genome, copyPos);
    genome->overwrite(301, copyVals);
    
    /* correct answer: genome of size 1000100
     with positions 0 to 4 having value 15,
     position 5 having value 30,
     positions 6 to 299 having value 15,
     position 300 having value 10,
     positions 301 to 350 having value 15,
     and positions 351 to 1000099 (end of genome) having values 10 */
    
    std::vector<std::byte> answer(1000100, (std::byte)10);
    for (int pos = 0; pos <= 350; pos++) {
        if(pos == 5)
            answer[pos] = (std::byte)30;
        else if (pos == 300)
            answer[pos] = (std::byte)10;
        else
            answer[pos] = (std::byte)15;
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    printResults(answer, genome, "Stress Test 4: ");
    
    delete genome;
}

/* 5000 overwrite mutations on size 500000 genome,
 doesn't initially rewrite entire genome (unlike stress tests 1-4),
 debug doesn't print entire genome since it's very large,
 user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest5(bool debug) {
    
    // all 500000 values are initially 0
    AbstractGenome* genome = new genomeName(500000);
    
    /* correct answer: genome of size 500000 with every value
     equal to 0 except every 100th value which is 25 */
    std::vector<std::byte> answer(genome->size(), (std::byte)0);
    
    /* 5000 overwrite mutations: point mutation on every 100th value
     to set that value to 25 */
    for (int i = 0; i < genome->size(); i+=100) {
        genome->overwrite(i, std::vector<std::byte>({(std::byte)25}));
        answer[i] = (std::byte)25; // update answer vector too
    }
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    printResults(answer, genome, "Stress Test 5: ");
    
    delete genome;
}

/* 1000 insert mutations of size 200 at position 0 in size 1 million genome,
 doesn't initially rewrite entire genome (unlike stress tests 1-4),
 debug doesn't print entire genome since it's very large,
 user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest6(bool debug) {
    
    size_t genomeSize = 1000000;
    
    // all 1 million values are initially 0
    AbstractGenome* genome = new genomeName(genomeSize);
    
    /* 1000 insertions of size 200
     with all values in the insertions equal to 55,
     inserting at the start of the genome */
    for (int i = 0; i < 1000; i++) {
        std::vector<std::byte> vals(200, (std::byte)55);
        genome->insert(0, vals);
    }
    
    /* correct answer: genome of size 1200000 with the first 200000
     indices having a value of 55 and the remaining 1 million indices
     having value 0 */
    std::vector<std::byte> answer_part_one(200000, (std::byte)55);
    std::vector<std::byte> answer_part_two(genomeSize, (std::byte)0);
    answer_part_two.insert(answer_part_two.begin(), answer_part_one.begin(), answer_part_one.end());
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    printResults(answer_part_two, genome, "Stress Test 6: ");
    
    delete genome;
}

/* 1000 remove mutations of size 200 at position 0 in size 1 million genome,
 doesn't initially rewrite entire genome (unlike stress tests 1-4),
 debug doesn't print entire genome since it's very large,
 user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest7(bool debug) {
    
    // all 1 million values are initially 0
    AbstractGenome* genome = new genomeName(1000000);
    
    /* 1000 deletions of size 200 at position 0,
     removing the first 200000 indices of the genome */
    for (int i = 0; i < 1000; i++) {
        genome->remove(0, 200);
    }
    
    /* correct answer: genome of size 800000 with all values equal to 0 */
    std::vector<std::byte> answer(800000, (std::byte)0);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    printResults(answer, genome, "Stress Test 7: ");
    
    delete genome;
}

/* multiple large-scale mutations (overwriting entire genome,
 5000 insertions, 5000 deletions) on size 500000 genome,
 debug doesn't print entire genome since it's very large,
 user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest8(bool debug) {
    // all 500000 values are initially 0
    AbstractGenome* genome = new genomeName(500000);
    
    // overwrite all 500000 values to 20
    genome->overwrite(0, std::vector<std::byte>(500000, (std::byte)20));
    
    // insert 5000 values of 30 at beginning of genome
    genome->insert(0, std::vector<std::byte>(5000, (std::byte)30));
    
    // remove 5000 values starting at position 2500
    genome->remove(2500, 5000);
    
    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }
    
    /* correct answer: genome of size 500000 with
     first 2500 values being 30 and remaining
     values being 20. */
    std::vector<std::byte> answer(500000, (std::byte)20);
    for (int i = 0; i < 2500; i++)
        answer[i] = (std::byte)30;
    
    printResults(answer, genome, "Stress Test 8: ");
    
    delete genome;
}

/* 5000 overwrite mutations on size 500000 genome, 
   doesn't initially rewrite entire genome (unlike stress tests 1-4),
   debug doesn't print entire genome since it's very large, 
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest5(bool debug) {

    // all 500000 values are initially 0
    AbstractGenome* genome = new genomeName(500000);

    /* correct answer: genome of size 500000 with every value 
       equal to 0 except every 100th value which is 25 */
    std::vector<std::byte> answer(genome->size(), (std::byte)0);

    /* 5000 overwrite mutations: point mutation on every 100th value
       to set that value to 25 */
    for (int i = 0; i < genome->size(); i+=100) {
         genome->overwrite(i, std::vector<std::byte>({(std::byte)25}));
         answer[i] = (std::byte)25; // update answer vector too
    }

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }

    printResults(answer, genome, "Stress Test 5: ");

    delete genome;
}

/* 1000 insert mutations of size 200 at position 0 in size 1 million genome, 
   doesn't initially rewrite entire genome (unlike stress tests 1-4),
   debug doesn't print entire genome since it's very large, 
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest6(bool debug) {

    size_t genomeSize = 1000000;

    // all 1 million values are initially 0
    AbstractGenome* genome = new genomeName(genomeSize);

    /* 1000 insertions of size 200 
       with all values in the insertions equal to 55,
       inserting at the start of the genome */
    for (int i = 0; i < 1000; i++) {
        std::vector<std::byte> vals(200, (std::byte)55);
        genome->insert(0, vals);
    }

    /* correct answer: genome of size 1200000 with the first 200000
       indices having a value of 55 and the remaining 1 million indices
       having value 0 */
    std::vector<std::byte> answer_part_one(200000, (std::byte)55);
    std::vector<std::byte> answer_part_two(genomeSize, (std::byte)0);
    answer_part_two.insert(answer_part_two.begin(), answer_part_one.begin(), answer_part_one.end());

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }

    printResults(answer_part_two, genome, "Stress Test 6: ");

    delete genome;
}

/* 1000 remove mutations of size 200 at position 0 in size 1 million genome, 
   doesn't initially rewrite entire genome (unlike stress tests 1-4),
   debug doesn't print entire genome since it's very large, 
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest7(bool debug) {

    // all 1 million values are initially 0
    AbstractGenome* genome = new genomeName(1000000);

    /* 1000 deletions of size 200 at position 0, 
       removing the first 200000 indices of the genome */
    for (int i = 0; i < 1000; i++) {
         genome->remove(0, 200);
    }

    /* correct answer: genome of size 800000 with all values equal to 0 */
    std::vector<std::byte> answer(800000, (std::byte)0);

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }

    printResults(answer, genome, "Stress Test 7: ");

    delete genome;
}

/* multiple large-scale mutations (overwriting entire genome, 
   5000 insertions, 5000 deletions) on size 500000 genome,
   debug doesn't print entire genome since it's very large, 
   user can modify test to print what they want when debug is set */
template <class genomeName>
void stressTest8(bool debug) {
    // all 500000 values are initially 0
    AbstractGenome* genome = new genomeName(500000);

    // overwrite all 500000 values to 20
    genome->overwrite(0, std::vector<std::byte>(500000, (std::byte)20));

    // insert 5000 values of 30 at beginning of genome
    genome->insert(0, std::vector<std::byte>(5000, (std::byte)30));

    // remove 5000 values starting at position 2500
    genome->remove(2500, 5000);

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }

    /* correct answer: genome of size 500000 with 
       first 2500 values being 30 and remaining 
       values being 20. */
    std::vector<std::byte> answer(500000, (std::byte)20);
    for (int i = 0; i < 2500; i++)
        answer[i] = (std::byte)30;
    
    printResults(answer, genome, "Stress Test 8: ");

    delete genome;
}

// runs all point and copy mutation tests
template <class genomeName>
void runOverwriteTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING OVERWRITE TESTS: " << std::endl;
    pointTest1<genomeName>(debug);
    pointTest2<genomeName>(debug);
    copyTest1<genomeName>(debug);
}

// runs all remove tests
template <class genomeName>
void runRemoveTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING REMOVE TESTS: " << std::endl;
    removeTest1<genomeName>(debug);
    removeTest2<genomeName>(debug);
    removeTest3<genomeName>(debug);
    removeTest4<genomeName>(debug);
    removeTest5<genomeName>(debug);
    removeTest6<genomeName>(debug);
    removeTest7<genomeName>(debug);
    removeTest8<genomeName>(debug);
    removeTest9<genomeName>(debug);
    removeTest10<genomeName>(debug);
    removeTest11<genomeName>(debug);
    removeTest12<genomeName>(debug);
    removeTest13<genomeName>(debug);
    removeTest14<genomeName>(debug);
}

// runs all insert tests
template <class genomeName>
void runInsertTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING INSERT TESTS: " << std::endl;
    insertTest1<genomeName>(debug);
    insertTest2<genomeName>(debug);
    insertTest3<genomeName>(debug);
    insertTest4<genomeName>(debug);
    insertTest5<genomeName>(debug);
    insertTest6<genomeName>(debug);
    insertTest7<genomeName>(debug);
    insertTest8<genomeName>(debug);
    insertTest9<genomeName>(debug);
    insertTest10<genomeName>(debug);
}

// run all clone tests
template <class genomeName>
void runCloneTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING CLONE TESTS: " << std::endl;
    cloneTest1<genomeName>(debug);
    cloneTest2<genomeName>(debug);
    cloneTest3<genomeName>(debug);
}

// runs all tests which test overwrite+insert+remove together
template <class genomeName>
void runAllMutationsTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING ALL-MUTATIONS TESTS: " << std::endl;
    allMutationsTest1<genomeName>(debug);
    allMutationsTest2<genomeName>(debug);
    allMutationsTest3<genomeName>(debug);
    allMutationsTest4<genomeName>(debug);
}


// runs all stress tests
template <class genomeName>
void runAllStressTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING STRESS TESTS: " << std::endl;
    
    auto t01 = std::chrono::steady_clock::now();
    stressTest1<genomeName>(debug);
    auto t11 = std::chrono::steady_clock::now();
    std::cout << "Stress test 1 took " << std::chrono::duration_cast<std::chrono::seconds>(t11 - t01).count() << " s" << std::endl;
    
    auto t02 = std::chrono::steady_clock::now();
    stressTest2<genomeName>(debug);
    auto t12 = std::chrono::steady_clock::now();
    std::cout << "Stress test 2 took " << std::chrono::duration_cast<std::chrono::seconds>(t12 - t02).count() << " s" << std::endl;
    
    auto t03 = std::chrono::steady_clock::now();
    stressTest3<genomeName>(debug);
    auto t13 = std::chrono::steady_clock::now();
    std::cout << "Stress test 3 took " << std::chrono::duration_cast<std::chrono::seconds>(t13 - t03).count() << " s" << std::endl;
    
    auto t04 = std::chrono::steady_clock::now();
    stressTest4<genomeName>(debug);
    stressTest5<genomeName>(debug);
    stressTest6<genomeName>(debug);
    stressTest7<genomeName>(debug);
    stressTest8<genomeName>(debug);
}

// runs all tests in this file
template <class genomeName>
void runUmaChangelogTests(bool debug) {
    runOverwriteTests<genomeName>(debug);
    runRemoveTests<genomeName>(debug);
    runInsertTests<genomeName>(debug);
    runCloneTests<genomeName>(debug);
    runAllMutationsTests<genomeName>(debug);
    runAllStressTests<genomeName>(debug);
}
