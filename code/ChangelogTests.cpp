#include "UmaGenome.h"
#include <cstddef>

// prints original genome, new genome, and changelog
// when debug is turned on
void printDebugOutput(UmaGenome* genome) {
    std::cout << "ORIGINAL GENOME: ";
    for (int index = 0; index < genome->size(); index++) {
        std::byte& num = GN::genomeRead<std::byte>(genome, index);
        std::cout << " " << (int)num;
    }
    std::cout << std::endl;

    std::cout << "\nCHANGELOG: " << std::endl;
    genome->printChangelog(); 

    std::cout << "\nCURRENT GENOME: ";
    genome->reconstructGenome();

    std::cout << "\n--------------------------" << std::endl;
}

// simple point mutations
void pointTest1(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    std::vector<int> answer{4,4,2,4,1,2,4,2,1,0};
    genome->pointMutate(1,4,true);
    genome->pointMutate(7,2,true);
    genome->pointMutate(3,4,true);

    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Point Test: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }
    delete genome;
}

// just one deletion
void deleteTest1(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->deleteMutate(5,3,true);

    std::vector<int> answer{4,3,2,0,1,1,0};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 1: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// only point and deletion mutations
void deleteTest2(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->pointMutate(1,1,true);
    genome->pointMutate(3,2,true);
    genome->pointMutate(7,2,true);
    genome->pointMutate(9,4,true);
    genome->deleteMutate(1,1,true);
    genome->deleteMutate(3,2,true);

    std::vector<int> answer{4,2,2,4,2,1,4};

    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 2: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// only point and deletion mutations
void deleteTest3(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->pointMutate(1,1,true);
    genome->pointMutate(3,2,true);
    genome->pointMutate(7,2,true);
    genome->pointMutate(9,4,true);
    genome->deleteMutate(3,2,true);

    std::vector<int> answer{4,1,2,2,4,2,1,4};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 3: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// delete a key that's already in map
void deleteTest4(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->pointMutate(1,1,true);
    genome->pointMutate(3,2,true);
    genome->pointMutate(4,3,true);
    genome->pointMutate(7,2,true);
    genome->pointMutate(9,4,true);
    genome->deleteMutate(1,1,true);
    genome->deleteMutate(3,2,true);

    std::vector<int> answer{4,2,2,4,2,1,4};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 4: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

void deleteTest5(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->pointMutate(2,3,true);
    genome->pointMutate(5,4,true);
    genome->deleteMutate(4,1,true);

    std::vector<int> answer{4,3,3,0,4,4,3,1,0};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 5: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// delete last element in genome
void deleteTest6(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->deleteMutate(9,1,true);

    std::vector<int> answer{4,3,2,0,1,2,4,3,1};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 6: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// delete first element in genome
void deleteTest7(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->deleteMutate(0,1,true);

    std::vector<int> answer{3,2,0,1,2,4,3,1,0};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 7: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// delete series of 4 elements starting at index 0
void deleteTest8(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->deleteMutate(0,4,true);

    std::vector<int> answer{1,2,4,3,1,0};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 8: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

// alternating point and delete mutations
void deleteTest9(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 0, 0, 0, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 4);
    GN::genomeWrite<uint8_t>(genome, 7, 3);
    GN::genomeWrite<uint8_t>(genome, 8, 1);
    GN::genomeWrite<uint8_t>(genome, 9, 0);

    genome->pointMutate(2,3,true);
    genome->deleteMutate(4,2,true);
    genome->pointMutate(5,4,true);
    genome->deleteMutate(5,3,true);
    genome->pointMutate(3,4,true);

    std::vector<int> answer{4,3,3,4,4};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 9: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

void runPointTests(bool debug) {
    std::cout << "--------------------------" << std::endl;
    std::cout << "RUNNING POINT TESTS: " << std::endl;
    pointTest1(debug);
}

void runDeleteTests(bool debug) {
   std::cout << "--------------------------" << std::endl;
   std::cout << "RUNNING DELETE TESTS: " << std::endl;
   deleteTest1(debug);
   deleteTest2(debug);
   deleteTest3(debug);
   deleteTest4(debug);
   deleteTest5(debug);
   deleteTest6(debug);
   deleteTest7(debug);
   deleteTest8(debug);
   deleteTest9(debug);
}

void runChangelogTests(bool debug) {
    runPointTests(debug);
    runDeleteTests(debug);
}