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

    std::cout << "\nOFFSET MAP: " << std::endl;
    genome->printOffsetMap();

    std::cout << "\nCURRENT GENOME: ";
    genome->reconstructGenome();

    std::cout << "\n--------------------------" << std::endl;
}

// simple point mutations
void pointTest1(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    std::vector<std::byte> answer{(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)4,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)2,(std::byte)1,(std::byte)0};
    genome->pointMutate(1, (std::byte)(4));
    genome->pointMutate(7, (std::byte)(2));
    genome->pointMutate(3, (std::byte)(4));

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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->deleteMutate(5,3);

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),(std::byte)(2),(std::byte)(0),(std::byte)(1),(std::byte)(1),(std::byte)(0)};
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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->pointMutate(1,(std::byte)(1));
    genome->pointMutate(3,(std::byte)(2));
    genome->pointMutate(7,(std::byte)(2));
    genome->pointMutate(9,(std::byte)(4));
    genome->deleteMutate(1,1);
    genome->deleteMutate(3,2);

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};

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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->pointMutate(1,(std::byte)(1));
    genome->pointMutate(3,(std::byte)(2));
    genome->pointMutate(7,(std::byte)(2));
    genome->pointMutate(9,(std::byte)(4));
    genome->deleteMutate(3,2);

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(1),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};
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
    UmaGenome* genome = new UmaGenome(10,5); 
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

    genome->pointMutate(1,(std::byte)(1));
    genome->pointMutate(3,(std::byte)(2));
    genome->pointMutate(4,(std::byte)(3));
    genome->pointMutate(7,(std::byte)(2));
    genome->pointMutate(9,(std::byte)(4));
    genome->deleteMutate(1,1);
    genome->deleteMutate(3,2);

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};
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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->pointMutate(2,(std::byte)(3));
    genome->pointMutate(5,(std::byte)(4));
    genome->deleteMutate(4,1);

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),(std::byte)3,(std::byte)0,(std::byte)4,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->deleteMutate(9,1);

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)2,(std::byte)0,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1};
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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->deleteMutate(0,1);

    std::vector<std::byte> answer{(std::byte)3,(std::byte)2,(std::byte)0,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->deleteMutate(0,4);

    std::vector<std::byte> answer{(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
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
    UmaGenome* genome = new UmaGenome(10, 5); 
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

    genome->pointMutate(2,(std::byte)3);
    genome->deleteMutate(4,2);
    genome->pointMutate(5,(std::byte)4);
    genome->deleteMutate(5,3);
    genome->pointMutate(3,(std::byte)4);

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)3,(std::byte)4,(std::byte)4};
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

void deleteTest10(bool debug) {
    UmaGenome* genome = new UmaGenome(10,100); 
    GN::genomeWrite<uint8_t>(genome, 0, 40);
    GN::genomeWrite<uint8_t>(genome, 1, 34);
    GN::genomeWrite<uint8_t>(genome, 2, 21);
    GN::genomeWrite<uint8_t>(genome, 3, 9);
    GN::genomeWrite<uint8_t>(genome, 4, 0);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 11);
    GN::genomeWrite<uint8_t>(genome, 7, 67);
    GN::genomeWrite<uint8_t>(genome, 8, 48);
    GN::genomeWrite<uint8_t>(genome, 9, 12);

    genome->pointMutate(4,(std::byte)70);
    genome->deleteMutate(0,1);
    genome->pointMutate(1,(std::byte)49);

    std::vector<std::byte> answer{(std::byte)34,(std::byte)49,(std::byte)9,(std::byte)70,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)12};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Delete Test 10: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;
}

void insertTest1(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 100); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    std::vector<std::byte> vals{(std::byte)(99)};
    genome->insertMutate(1,vals);

    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Insert Test 1: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

void insertTest2(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 100); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insertMutate(1,vals);

    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,(std::byte)66,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Insert Test 2: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

// insert at beginning of genome
void insertTest3(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 100); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insertMutate(0,vals);

    std::vector<std::byte> answer{(std::byte)99,(std::byte)66,(std::byte)11,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Insert Test 3: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

// insert at end of genome
void insertTest4(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 100); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insertMutate(5,vals);

    std::vector<std::byte> answer{(std::byte)11,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55,(std::byte)99,(std::byte)66};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Insert Test 4: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

// point and insert mutations
void insertTest5(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);

    genome->pointMutate(1,(std::byte)1);
    genome->pointMutate(3,(std::byte)2);
    genome->pointMutate(4,(std::byte)3);
    genome->insertMutate(1,{(std::byte)(3)});
    genome->insertMutate(3,{(std::byte)(4), (std::byte)(4)});

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)2,(std::byte)3};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "Insert Test 5: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

// point,insert,and delete mutations
void allMutationsTest1(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);

    // 4 3 2 0 1
    // 4 1 2 0 1
    // 4 1 2 2 1
    // 4 1 2 2 3
    // 4 3 1 2 2 3
    // 4 3 1 4 4 2 2 3
    // 4 3 1 4 2 2 2 3
    // 4 3 1 4 2 3
    genome->pointMutate(1,(std::byte)1);
    genome->pointMutate(3,(std::byte)2);
    genome->pointMutate(4,(std::byte)3);
    genome->insertMutate(1,{(std::byte)(3)});
    genome->insertMutate(3,{(std::byte)(4), (std::byte)(4)});
    genome->pointMutate(4,(std::byte)2);
    genome->deleteMutate(5,2);


    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)2,(std::byte)3};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "All-Mutations Test 1: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

// point,insert,and delete mutations
void allMutationsTest2(bool debug) {
    UmaGenome* genome = new UmaGenome(10, 100); 
    GN::genomeWrite<uint8_t>(genome, 0, 40);
    GN::genomeWrite<uint8_t>(genome, 1, 34);
    GN::genomeWrite<uint8_t>(genome, 2, 21);
    GN::genomeWrite<uint8_t>(genome, 3, 9);
    GN::genomeWrite<uint8_t>(genome, 4, 0);
    GN::genomeWrite<uint8_t>(genome, 5, 2);
    GN::genomeWrite<uint8_t>(genome, 6, 11);
    GN::genomeWrite<uint8_t>(genome, 7, 67);
    GN::genomeWrite<uint8_t>(genome, 8, 48);
    GN::genomeWrite<uint8_t>(genome, 9, 12);

    // parent: 40,34,21,9,0,2,11,67,48,12
    // after mutation 1: 60,50,51,40,34,21,9,0,2,11,67,48,12 - CORRECT
    // after mutation 2: 60,50,51,40,70,21,9,0,2,11,67,48,12 - GOES WRONG HERE
    // after mutation 3: 50,51,40,70,21,9,0,2,11,67,48,12
    // after mutation 4: 50,51,40,70,21,9,0,2,11,67,48,12,62,52,42
    // after mutation 5: 50,51,40,70,21,9,0,2,11,67,48,12,42
    // after mutation 6: 50,51,40,99,70,21,9,0,2,11,67,48,12,42
    // after mutation 7: 50,51,40,99,70,21,9,0,2,11,67,48,42
    // after mutation 8: 50,49,40,99,70,21,9,0,2,11,67,48,42
    // current: 50,49,40,99,70,21,9,0,2,11,67,48,42

    genome->insertMutate(0,{(std::byte)(60), (std::byte)(50), (std::byte)(51)});
    genome->pointMutate(4,(std::byte)70);
    genome->deleteMutate(0,1);
    genome->insertMutate(12,{(std::byte)(62), (std::byte)(52), (std::byte)(42)});
    genome->deleteMutate(12,2);
    genome->insertMutate(3,{(std::byte)(99)});
    genome->deleteMutate(12,1);
    genome->pointMutate(1,(std::byte)49);

    std::vector<std::byte> answer{(std::byte)50,(std::byte)49,(std::byte)40,(std::byte)99,(std::byte)70,(std::byte)21,(std::byte)9,(std::byte)0,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)42};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    for(int i = 0; i < answer.size() && !failed; i++) {
        if(genome->getCurrentGenomeAt(i) != answer[i]){
            failed = true;
        }
    }

    std::cout << "All-Mutations Test 2: ";
    if(failed) 
        std::cout << "failed\n";
    else
        std::cout << "passed\n";

    if(debug){
        printDebugOutput(genome);
    }

    delete genome;

}

void demoTest(bool debug) {
    UmaGenome* genome = new UmaGenome(5, 100); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    genome->insertMutate(1, {(std::byte)(99)});
    genome->deleteMutate(2, 2);
    genome->pointMutate(2, (std::byte)(65));
    genome->insertMutate(2, {(std::byte)(88), (std::byte)(77)});

    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(99),(std::byte)(88),(std::byte)(77),(std::byte)(65),(std::byte)(55)};
    bool failed = false;
    if (genome->getCurrentGenomeSize() != answer.size())
        failed = true;
    else {
        for(int i = 0; i < answer.size() && !failed; i++) {
            if(genome->getCurrentGenomeAt(i) != answer[i]){
                failed = true;
            }
        }
    }

    std::cout << "Demo Test: ";
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
   deleteTest10(debug);
}

void runInsertTests(bool debug) {
   std::cout << "--------------------------" << std::endl;
   std::cout << "RUNNING INSERT TESTS: " << std::endl;
   insertTest1(debug);
   insertTest2(debug);
   insertTest3(debug);
   insertTest4(debug);
   insertTest5(debug);
}

void runAllMutationsTests(bool debug) {
   std::cout << "--------------------------" << std::endl;
   std::cout << "RUNNING ALL-MUTATIONS TESTS: " << std::endl;
   allMutationsTest1(debug);
   allMutationsTest2(debug);
}

void runChangelogTests(bool debug) {
    runPointTests(debug);
    runDeleteTests(debug);
    runInsertTests(debug);
    runAllMutationsTests(debug);
}