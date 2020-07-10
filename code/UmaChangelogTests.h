#include "AbstractGenome.h"
#include <string>
#include <cstddef>
#include <vector>

/* TO RUN THESE TESTS ON ANY GENOME:
   - in the main.cpp file, include "UmaChangelogTests.h"
   - in the main method, call runUmaChangelogTests<genomeName>(debug),
     where genomeName is the name of the genome class you want to test
     and you can pass in true or false for debug based on if you want
     to see the debug output */

// checks if test passes or fails
void printResults(std::vector<std::byte>& answer, AbstractGenome* gen, std::string testName) {
    bool failed = false;
    if (gen->size() != answer.size())
        failed = true;
    else {
        std::byte* genomeData = gen->data(0,0);
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
template <class genomeName>
void pointTest1(bool debug) {
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
    }
    
    std::vector<std::byte> answer{(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)4,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)2,(std::byte)1,(std::byte)0};
    genome->overwrite(1, std::vector<std::byte>({(std::byte)(4)}));
    genome->overwrite(7, std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(3, std::vector<std::byte>({(std::byte)(4)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    printResults(answer, genome, "Point Test 1: ");
    
    delete genome;
}

// just one deletion
template <class genomeName>
void deleteTest1(bool debug) {
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
template <class genomeName>
void deleteTest2(bool debug) {
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
    }

    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
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
template <class genomeName>
void deleteTest3(bool debug) {
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
    }

    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
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
template <class genomeName>
void deleteTest4(bool debug) {
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
    }

    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(4,std::vector<std::byte>({(std::byte)(3)}));
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
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

template <class genomeName>
void deleteTest5(bool debug) {
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
    }

    genome->overwrite(2,std::vector<std::byte>({(std::byte)(3)}));
    genome->overwrite(5,std::vector<std::byte>({(std::byte)(4)}));
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
template <class genomeName>
void deleteTest6(bool debug) {
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
template <class genomeName>
void deleteTest7(bool debug) {
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
template <class genomeName>
void deleteTest8(bool debug) {
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
template <class genomeName>
void deleteTest9(bool debug) {
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
    }

    genome->overwrite(2,std::vector<std::byte>({(std::byte)3}));
    genome->remove(4,2);
    genome->overwrite(5,std::vector<std::byte>({(std::byte)4}));
    genome->remove(5,3);
    genome->overwrite(3,std::vector<std::byte>({(std::byte)4}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)3,(std::byte)4,(std::byte)4};
    
    printResults(answer, genome, "Delete Test 9: ");

    delete genome;
}

template <class genomeName>
void deleteTest10(bool debug) {
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
    }

    genome->overwrite(4,std::vector<std::byte>({(std::byte)70}));
    genome->remove(0,1);
    genome->overwrite(1,std::vector<std::byte>({(std::byte)49}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)34,(std::byte)49,(std::byte)9,(std::byte)70,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)12};
    
    printResults(answer, genome, "Delete Test 10: ");

    delete genome;
}

template <class genomeName>
void insertTest1(bool debug) {
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

template <class genomeName>
void insertTest2(bool debug) {
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
template <class genomeName>
void insertTest3(bool debug) {
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
template <class genomeName>
void insertTest4(bool debug) {
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
template <class genomeName>
void insertTest5(bool debug) {
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
    }

    genome->overwrite(1,std::vector<std::byte>({(std::byte)1}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)2}));
    genome->overwrite(4,std::vector<std::byte>({(std::byte)3}));
    genome->insert(1,std::vector<std::byte>({(std::byte)(3)}));
    genome->insert(3,std::vector<std::byte>({(std::byte)(4), (std::byte)(4)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "Insert Test 5: ");

    delete genome;
}

// point,insert,and delete mutations
template <class genomeName>
void allMutationsTest1(bool debug) {
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
    }

    // 4 3 2 0 1
    // 4 1 2 0 1
    // 4 1 2 2 1
    // 4 1 2 2 3
    // 4 3 1 2 2 3
    // 4 3 1 4 4 2 2 3
    // 4 3 1 4 2 2 2 3
    // 4 3 1 4 2 3
    genome->overwrite(1,std::vector<std::byte>({(std::byte)1}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)2}));
    genome->overwrite(4,std::vector<std::byte>({(std::byte)3}));
    genome->insert(1,std::vector<std::byte>({(std::byte)(3)}));
    genome->insert(3,std::vector<std::byte>({(std::byte)(4), (std::byte)(4)}));
    genome->overwrite(4,std::vector<std::byte>({(std::byte)2}));
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
template <class genomeName>
void allMutationsTest2(bool debug) {
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

    genome->insert(0,std::vector<std::byte>({(std::byte)(60), (std::byte)(50), (std::byte)(51)}));
    genome->overwrite(4,std::vector<std::byte>({(std::byte)70}));
    genome->remove(0,1);
    genome->insert(12,std::vector<std::byte>({(std::byte)(62), (std::byte)(52), (std::byte)(42)}));
    genome->remove(12,2);
    genome->insert(3,std::vector<std::byte>({(std::byte)(99)}));
    genome->remove(12,1);
    genome->overwrite(1,std::vector<std::byte>({(std::byte)49}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)50,(std::byte)49,(std::byte)40,(std::byte)99,(std::byte)70,(std::byte)21,(std::byte)9,(std::byte)0,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)42};
    
    printResults(answer, genome, "All-Mutations Test 2: ");

    delete genome;
}

// example from powerpoint demo
template <class genomeName>
void allMutationsTest3(bool debug) {
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
    }

    genome->insert(1, std::vector<std::byte>({(std::byte)(99)}));
    genome->remove(2, 2);
    genome->overwrite(2, std::vector<std::byte>({(std::byte)(65)}));
    genome->insert(2, std::vector<std::byte>({(std::byte)(88), (std::byte)(77)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(99),(std::byte)(88),(std::byte)(77),(std::byte)(65),(std::byte)(55)};
    
    printResults(answer, genome, "All-Mutations Test 3: ");

    delete genome;
}

// multiple point mutations on a large genome
template <class genomeName>
void stressTest1(bool debug) {
    AbstractGenome* genome = new genomeName(100); 
    for (int i = 0; i < 100; i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    genome->overwrite(10, std::vector<std::byte>({(std::byte)(65)}));
    genome->overwrite(20, std::vector<std::byte>({(std::byte)(75)}));
    genome->overwrite(30, std::vector<std::byte>({(std::byte)(85)}));
    genome->overwrite(40, std::vector<std::byte>({(std::byte)(95)}));
    genome->overwrite(50, std::vector<std::byte>({(std::byte)(105)}));
    genome->overwrite(60, std::vector<std::byte>({(std::byte)(205)}));
    genome->overwrite(70, std::vector<std::byte>({(std::byte)(15)}));
    genome->overwrite(80, std::vector<std::byte>({(std::byte)(20)}));
    genome->overwrite(90, std::vector<std::byte>({(std::byte)(45)}));
    genome->overwrite(99, std::vector<std::byte>({(std::byte)(70)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

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

    printResults(answer, genome, "Stress Test 1: ");

    delete genome;
}

// insertion inception (insertion inside insertion inside insertion...)
template <class genomeName>
void stressTest2(bool debug) {
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
    }

    // original: 17 20 10 11 32
    // after mutation 1: 17 20 10 81 82 83 11 32
    // after mutation 2: 17 20 10 81 81 82 83 82 83 11 32
    // after mutation 3: 17 20 10 81 81 71 72 73 82 83 82 83 11 32
    // after mutation 4: 17 20 10 81 81 71 61 62 72 73 82 83 82 83 11 32
    // after mutation 5: 17 20 10 81 81 71 61 51 62 72 73 82 83 82 83 11 32
    genome->insert(3, std::vector<std::byte>({(std::byte)81,(std::byte)82, (std::byte)83}));
    genome->insert(4, std::vector<std::byte>({(std::byte)81,(std::byte)82, (std::byte)83}));
    genome->insert(5, std::vector<std::byte>({(std::byte)71,(std::byte)72, (std::byte)73}));
    genome->insert(6, std::vector<std::byte>({(std::byte)61,(std::byte)62}));
    genome->insert(7, std::vector<std::byte>({(std::byte)51}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer{(std::byte)(17),(std::byte)(20),(std::byte)(10),
                                    (std::byte)(81),(std::byte)(81),(std::byte)(71), 
                                    (std::byte)(61), (std::byte)(51), (std::byte)(62), 
                                    (std::byte)(72), (std::byte)(73), (std::byte)(82),
                                    (std::byte)(83), (std::byte)(82), (std::byte)(83),
                                    (std::byte)(11), (std::byte)(32)};
    printResults(answer, genome, "Stress Test 2: ");

    delete genome;
}

// large insertion in large genome
template <class genomeName>
void stressTest3(bool debug) {
    AbstractGenome* genome = new genomeName(100);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)50);
    } 

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    // insert 50 values, all of which are 44
    std::vector<std::byte> vals(50, (std::byte)44);
    genome->insert(1, vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer(150, (std::byte)50);
    for (int a = 1; a <= 50; a++) {
        answer[a] = (std::byte)44;
    }

    printResults(answer, genome, "Stress Test 3: ");

    delete genome;
}

// large deletion for large genome
template <class genomeName>
void stressTest4(bool debug) {
    AbstractGenome* genome = new genomeName(200);
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    } 

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
    }

    // delete 50 values
    genome->remove(1, 50);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
    }

    std::vector<std::byte> answer(150);
    answer[0] = (std::byte)0;
    for (int a = 1; a < answer.size(); a++) {
        answer[a] = (std::byte)(50+a);
    }

    printResults(answer, genome, "Stress Test 4: ");

    delete genome;
}

template <class genomeName>
void runPointTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING POINT TESTS: " << std::endl;
    pointTest1<genomeName>(debug);
}

template <class genomeName>
void runDeleteTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING DELETE TESTS: " << std::endl;
    deleteTest1<genomeName>(debug);
    deleteTest2<genomeName>(debug);
    deleteTest3<genomeName>(debug);
    deleteTest4<genomeName>(debug);
    deleteTest5<genomeName>(debug);
    deleteTest6<genomeName>(debug);
    deleteTest7<genomeName>(debug);
    deleteTest8<genomeName>(debug);
    deleteTest9<genomeName>(debug);
    deleteTest10<genomeName>(debug);
}

template <class genomeName>
void runInsertTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING INSERT TESTS: " << std::endl;
    insertTest1<genomeName>(debug);
    insertTest2<genomeName>(debug);
    insertTest3<genomeName>(debug);
    insertTest4<genomeName>(debug);
    insertTest5<genomeName>(debug);
}

template <class genomeName>
void runAllMutationsTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING ALL-MUTATIONS TESTS: " << std::endl;
    allMutationsTest1<genomeName>(debug);
    allMutationsTest2<genomeName>(debug);
    allMutationsTest3<genomeName>(debug);
}

template <class genomeName>
void runAllStressTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING STRESS TESTS: " << std::endl;
    stressTest1<genomeName>(debug);
    stressTest2<genomeName>(debug);
    stressTest3<genomeName>(debug);
    stressTest4<genomeName>(debug);
}

template <class genomeName>
void runUmaChangelogTests(bool debug) {
    runPointTests<genomeName>(debug);
    runDeleteTests<genomeName>(debug);
    runInsertTests<genomeName>(debug);
    runAllMutationsTests<genomeName>(debug);
    runAllStressTests<genomeName>(debug);
}