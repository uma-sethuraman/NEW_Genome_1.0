#include "UmaGenome.h"
#include <cstddef>
#include <string>
#include "AbstractGenome.h"

void printResults(std::vector<std::byte> answer, AbstractGenome* gen, std::string testName) {
    bool failed = false;
    if (gen->size() != answer.size())
        failed = true;
    else {
        auto genomeData = gen->data(0,0);
        for(int i = 0; i < answer.size() && !failed; i++) {
            if(genomeData[i] != answer[i]){
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

// simple point mutations
void pointTest1(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }
    
    std::vector<std::byte> answer{(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)4,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)2,(std::byte)1,(std::byte)0};
    genome->overwrite(1, {(std::byte)(4)});
    genome->overwrite(7, {(std::byte)(2)});
    genome->overwrite(3, {(std::byte)(4)});

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    printResults(answer, genome, "Point Test 1: ");
    
    delete genome;
}

// just one deletion
void deleteTest1(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->remove(5,3);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),(std::byte)(2),(std::byte)(0),(std::byte)(1),(std::byte)(1),(std::byte)(0)};
    
    printResults(answer, genome, "Delete Test 1: ");

    delete genome;
}

// only point and deletion mutations
void deleteTest2(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(1,{(std::byte)(1)});
    genome->overwrite(3,{(std::byte)(2)});
    genome->overwrite(7,{(std::byte)(2)});
    genome->overwrite(9,{(std::byte)(4)});
    genome->remove(1,1);
    genome->remove(3,2);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};

    printResults(answer, genome, "Delete Test 2: ");

    delete genome;
}

// only point and deletion mutations
void deleteTest3(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(1,{(std::byte)(1)});
    genome->overwrite(3,{(std::byte)(2)});
    genome->overwrite(7,{(std::byte)(2)});
    genome->overwrite(9,{(std::byte)(4)});
    genome->remove(3,2);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(1),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};
    
    printResults(answer, genome, "Delete Test 3: ");

    delete genome;
}

// delete a key that's already in map
void deleteTest4(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(1,{(std::byte)(1)});
    genome->overwrite(3,{(std::byte)(2)});
    genome->overwrite(4,{(std::byte)(3)});
    genome->overwrite(7,{(std::byte)(2)});
    genome->overwrite(9,{(std::byte)(4)});
    genome->remove(1,1);
    genome->remove(3,2);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};
    
    printResults(answer, genome, "Delete Test 4: ");

    delete genome;
}

void deleteTest5(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(2,{(std::byte)(3)});
    genome->overwrite(5,{(std::byte)(4)});
    genome->remove(4,1);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),(std::byte)3,(std::byte)0,(std::byte)4,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Delete Test 5: ");

    delete genome;
}

// delete last element in genome
void deleteTest6(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->remove(9,1);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)2,(std::byte)0,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1};
    
    printResults(answer, genome, "Delete Test 6: ");

    delete genome;
}

// delete first element in genome
void deleteTest7(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->remove(0,1);
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)3,(std::byte)2,(std::byte)0,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Delete Test 7: ");

    delete genome;
}

// delete series of 4 elements starting at index 0
void deleteTest8(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->remove(0,4);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Delete Test 8: ");

    delete genome;
}

// alternating point and delete mutations
void deleteTest9(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(2,{(std::byte)3});
    genome->remove(4,2);
    genome->overwrite(5,{(std::byte)4});
    genome->remove(5,3);
    genome->overwrite(3,{(std::byte)4});

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)3,(std::byte)4,(std::byte)4};
    
    printResults(answer, genome, "Delete Test 9: ");

    delete genome;
}

void deleteTest10(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(4,{(std::byte)70});
    genome->remove(0,1);
    genome->overwrite(1,{(std::byte)49});

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)34,(std::byte)49,(std::byte)9,(std::byte)70,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)12};
    
    printResults(answer, genome, "Delete Test 10: ");

    delete genome;
}

void insertTest1(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    std::vector<std::byte> vals{(std::byte)(99)};
    genome->insert(1,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 1: ");

    delete genome;
}

void insertTest2(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(1,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,(std::byte)66,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 2: ");

    delete genome;
}

// insert at beginning of genome
void insertTest3(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(0,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)99,(std::byte)66,(std::byte)11,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 3: ");

    delete genome;
}

// insert at end of genome
void insertTest4(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(5,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)11,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55,(std::byte)99,(std::byte)66};
    
    printResults(answer, genome, "Insert Test 4: ");

    delete genome;
}

// point and insert mutations
void insertTest5(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(1,{(std::byte)1});
    genome->overwrite(3,{(std::byte)2});
    genome->overwrite(4,{(std::byte)3});
    genome->insert(1,{(std::byte)(3)});
    genome->insert(3,{(std::byte)(4), (std::byte)(4)});

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "Insert Test 5: ");

    delete genome;
}

// point,insert,and delete mutations
void allMutationsTest1(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 4);
    GN::genomeWrite<uint8_t>(genome, 1, 3);
    GN::genomeWrite<uint8_t>(genome, 2, 2);
    GN::genomeWrite<uint8_t>(genome, 3, 0);
    GN::genomeWrite<uint8_t>(genome, 4, 1);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    // 4 3 2 0 1
    // 4 1 2 0 1
    // 4 1 2 2 1
    // 4 1 2 2 3
    // 4 3 1 2 2 3
    // 4 3 1 4 4 2 2 3
    // 4 3 1 4 2 2 2 3
    // 4 3 1 4 2 3
    genome->overwrite(1,{(std::byte)1});
    genome->overwrite(3,{(std::byte)2});
    genome->overwrite(4,{(std::byte)3});
    genome->insert(1,{(std::byte)(3)});
    genome->insert(3,{(std::byte)(4), (std::byte)(4)});
    genome->overwrite(4,{(std::byte)2});
    genome->remove(5,2);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "All-Mutations Test 1: ");

    delete genome;
}

// point,insert,and delete mutations
void allMutationsTest2(bool debug) {
    AbstractGenome* genome = new UmaGenome(10); 
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

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

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

    genome->insert(0,{(std::byte)(60), (std::byte)(50), (std::byte)(51)});
    genome->overwrite(4,{(std::byte)70});
    genome->remove(0,1);
    genome->insert(12,{(std::byte)(62), (std::byte)(52), (std::byte)(42)});
    genome->remove(12,2);
    genome->insert(3,{(std::byte)(99)});
    genome->remove(12,1);
    genome->overwrite(1,{(std::byte)49});

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)50,(std::byte)49,(std::byte)40,(std::byte)99,(std::byte)70,(std::byte)21,(std::byte)9,(std::byte)0,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)42};
    
    printResults(answer, genome, "All-Mutations Test 2: ");

    delete genome;

}

// example from powerpoint demo
void allMutationsTest3(bool debug) {
    AbstractGenome* genome = new UmaGenome(5); 
    GN::genomeWrite<uint8_t>(genome, 0, 11);
    GN::genomeWrite<uint8_t>(genome, 1, 22);
    GN::genomeWrite<uint8_t>(genome, 2, 33);
    GN::genomeWrite<uint8_t>(genome, 3, 44);
    GN::genomeWrite<uint8_t>(genome, 4, 55);

    if (debug) {
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->insert(1, {(std::byte)(99)});
    genome->remove(2, 2);
    genome->overwrite(2, {(std::byte)(65)});
    genome->insert(2, {(std::byte)(88), (std::byte)(77)});

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(99),(std::byte)(88),(std::byte)(77),(std::byte)(65),(std::byte)(55)};
    
    printResults(answer, genome, "All-Mutations Test 3: ");

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
   allMutationsTest3(debug);
}

void runChangelogTests(bool debug) {
    runPointTests(debug);
    runDeleteTests(debug);
    runInsertTests(debug);
    runAllMutationsTests(debug);
}