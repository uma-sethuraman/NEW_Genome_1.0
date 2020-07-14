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
        for(int i = 0; i < answer.size(); i++) {
            if((*(genomeData+i)) != answer[i]){
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
    
    std::vector<std::byte> answer{(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)4,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)2,(std::byte)1,(std::byte)0};
    genome->overwrite(1, std::vector<std::byte>({(std::byte)(4)}));
    genome->overwrite(7, std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(3, std::vector<std::byte>({(std::byte)(4)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    printResults(answer, genome, "Point Test 1: ");
    
    delete genome;
}

// copy mutation on size 50 genome
template <class genomeName>
void copyTest1(bool debug) {
    AbstractGenome* genome = new genomeName(50); 
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "ORIGINAL GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    // replace the first 10 values with the last 10 values
    // first 10 values should become: 40 41 42 43 44 45 46 47 48 49
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

// deletion in the middle of size 10 genome
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
        std::cout << std::endl;
    }

    genome->remove(5,3);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),(std::byte)(2),(std::byte)(0),(std::byte)(1),(std::byte)(1),(std::byte)(0)};
    
    printResults(answer, genome, "Delete Test 1: ");

    delete genome;
}

// point and deletion mutations on size 10 genome
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
        std::cout << std::endl;
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
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};

    printResults(answer, genome, "Delete Test 2: ");

    delete genome;
}

// point and deletion mutations on size 10 genome
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
        std::cout << std::endl;
    }

    genome->overwrite(1,std::vector<std::byte>({(std::byte)(1)}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(7,std::vector<std::byte>({(std::byte)(2)}));
    genome->overwrite(9,std::vector<std::byte>({(std::byte)(4)}));
    genome->remove(3,2);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(1),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};
    
    printResults(answer, genome, "Delete Test 3: ");

    delete genome;
}

// deleting indices that were previously altered by point mutations
// size 10 genome
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
        std::cout << std::endl;
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
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(2),(std::byte)(2),(std::byte)(4),(std::byte)(2),(std::byte)(1),(std::byte)(4)};
    
    printResults(answer, genome, "Delete Test 4: ");

    delete genome;
}

// point and deletion mutations on size 10 genome
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
        std::cout << std::endl;
    }

    genome->overwrite(2,std::vector<std::byte>({(std::byte)(3)}));
    genome->overwrite(5,std::vector<std::byte>({(std::byte)(4)}));
    genome->remove(4,1);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)(4),(std::byte)(3),(std::byte)3,(std::byte)0,(std::byte)4,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Delete Test 5: ");

    delete genome;
}

// delete last element in size 10 genome
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
        std::cout << std::endl;
    }

    genome->remove(9,1);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)2,(std::byte)0,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1};
    
    printResults(answer, genome, "Delete Test 6: ");

    delete genome;
}

// delete first element in size 10 genome
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
        std::cout << std::endl;
    }

    genome->remove(0,1);
    
    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)3,(std::byte)2,(std::byte)0,(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Delete Test 7: ");

    delete genome;
}

// delete series of 4 elements starting at index 0
// size 10 genome
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
        std::cout << std::endl;
    }

    genome->remove(0,4);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)1,(std::byte)2,(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)0};
    
    printResults(answer, genome, "Delete Test 8: ");

    delete genome;
}

// alternating point and delete mutations on size 10 genome
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
        std::cout << std::endl;
    }

    genome->overwrite(2,std::vector<std::byte>({(std::byte)3}));
    genome->remove(4,2);
    genome->overwrite(5,std::vector<std::byte>({(std::byte)4}));
    genome->remove(5,3);
    genome->overwrite(3,std::vector<std::byte>({(std::byte)4}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)3,(std::byte)4,(std::byte)4};
    
    printResults(answer, genome, "Delete Test 9: ");

    delete genome;
}

// alternating point and delete mutations on size 10 genome
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
        std::cout << std::endl;
    }

    genome->overwrite(4,std::vector<std::byte>({(std::byte)70}));
    genome->remove(0,1);
    genome->overwrite(1,std::vector<std::byte>({(std::byte)49}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)34,(std::byte)49,(std::byte)9,(std::byte)70,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)12};
    
    printResults(answer, genome, "Delete Test 10: ");

    delete genome;
}

// inserting one value at position 1 of size 5 genome
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
        std::cout << std::endl;
    }

    std::vector<std::byte> vals{(std::byte)(99)};
    genome->insert(1,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 1: ");

    delete genome;
}

// insertion of two values at position 1 of size 5 genome
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
        std::cout << std::endl;
    }

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(1,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)11,(std::byte)99,(std::byte)66,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 2: ");

    delete genome;
}

// insert at beginning of size 5 genome
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
        std::cout << std::endl;
    }

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(0,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)99,(std::byte)66,(std::byte)11,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55};
    
    printResults(answer, genome, "Insert Test 3: ");

    delete genome;
}

// insert at end of size 5 genome
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
        std::cout << std::endl;
    }

    std::vector<std::byte> vals{(std::byte)(99), (std::byte)(66)};
    genome->insert(5,vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)11,(std::byte)22,(std::byte)33,(std::byte)44,(std::byte)55,(std::byte)99,(std::byte)66};
    
    printResults(answer, genome, "Insert Test 4: ");

    delete genome;
}

// point and insert mutations on size 5 genome
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
        std::cout << std::endl;
    }

    genome->overwrite(1,std::vector<std::byte>({(std::byte)1}));
    genome->overwrite(3,std::vector<std::byte>({(std::byte)2}));
    genome->overwrite(4,std::vector<std::byte>({(std::byte)3}));
    genome->insert(1,std::vector<std::byte>({(std::byte)(3)}));
    genome->insert(3,std::vector<std::byte>({(std::byte)(4), (std::byte)(4)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)4,(std::byte)2,(std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "Insert Test 5: ");

    delete genome;
}

// overwrite, insert, and remove mutations on size 5 genome
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
        std::cout << std::endl;
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
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)4,(std::byte)3,(std::byte)1,(std::byte)4,(std::byte)2,(std::byte)3};
    
    printResults(answer, genome, "All-Mutations Test 1: ");

    delete genome;
}

// overwrite, insert, and remove mutations on size 10 genome
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
        std::cout << std::endl;
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
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)50,(std::byte)49,(std::byte)40,(std::byte)99,(std::byte)70,(std::byte)21,(std::byte)9,(std::byte)0,(std::byte)2,(std::byte)11,(std::byte)67,(std::byte)48,(std::byte)42};
    
    printResults(answer, genome, "All-Mutations Test 2: ");

    delete genome;
}

// example from powerpoint demo
// overwrite, insert, and remove mutations on size 5 genome
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
        std::cout << std::endl;
    }

    genome->insert(1, std::vector<std::byte>({(std::byte)(99)}));
    genome->remove(2, 2);
    genome->overwrite(2, std::vector<std::byte>({(std::byte)(65)}));
    genome->insert(2, std::vector<std::byte>({(std::byte)(88), (std::byte)(77)}));

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer{(std::byte)(11),(std::byte)(99),(std::byte)(88),(std::byte)(77),(std::byte)(65),(std::byte)(55)};
    
    printResults(answer, genome, "All-Mutations Test 3: ");

    delete genome;
}

// multiple point mutations on size 100 genome
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
        std::cout << std::endl;
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
        std::cout << std::endl;
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
// size 5 genome
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
        std::cout << std::endl;
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
        std::cout << std::endl;
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

// insertion of size 50 on size 100 genome
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
        std::cout << std::endl;
    }

    // insert 50 values, all of which are 44
    std::vector<std::byte> vals(50, (std::byte)44);
    genome->insert(1, vals);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer(150, (std::byte)50);
    for (int a = 1; a <= 50; a++) {
        answer[a] = (std::byte)44;
    }

    printResults(answer, genome, "Stress Test 3: ");

    delete genome;
}

// deletion of size 50 for size 200 genome
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
        std::cout << std::endl;
    }

    // delete 50 values
    genome->remove(1, 50);

    if (debug) {
        std::cout << "CURRENT GENOME: ";
        genome->show();
        std::cout << std::endl;
    }

    std::vector<std::byte> answer(150);
    answer[0] = (std::byte)0;
    for (int a = 1; a < answer.size(); a++) {
        answer[a] = (std::byte)(50+a);
    }

    printResults(answer, genome, "Stress Test 4: ");

    delete genome;
}

// multiple point mutations on size 100000 large genome
// debug doesn't print entire genome since it's very large
// user can modify test to print what they want when debug is set
template <class genomeName>
void stressTest5(bool debug) {
    AbstractGenome* genome = new genomeName(100000); 
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)i);
    }

    genome->overwrite(100, std::vector<std::byte>({(std::byte)(65)}));
    genome->overwrite(200, std::vector<std::byte>({(std::byte)(75)}));
    genome->overwrite(300, std::vector<std::byte>({(std::byte)(85)}));
    genome->overwrite(4000, std::vector<std::byte>({(std::byte)(95)}));
    genome->overwrite(5000, std::vector<std::byte>({(std::byte)(105)}));
    genome->overwrite(6000, std::vector<std::byte>({(std::byte)(205)}));
    genome->overwrite(70000, std::vector<std::byte>({(std::byte)(15)}));
    genome->overwrite(80000, std::vector<std::byte>({(std::byte)(20)}));
    genome->overwrite(90000, std::vector<std::byte>({(std::byte)(45)}));
    genome->overwrite(99999, std::vector<std::byte>({(std::byte)(70)}));

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }

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

    printResults(answer, genome, "Stress Test 5: ");

    delete genome;
}

// insertion of size 300 at beginning of size 100000 genome
// debug doesn't print entire genome since it's very large
// user can modify test to print what they want when debug is set
template <class genomeName>
void stressTest6(bool debug) {
    AbstractGenome* genome = new genomeName(100000); 
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)17);
    }

    // insert 300 values, all of which are 77
    std::vector<std::byte> vals(300, (std::byte)66);
    genome->insert(0, vals);

    if (debug) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Debug output must be set by user, because of very large size genome.\n" << std::endl;
    }

    std::vector<std::byte> answer(100300, (std::byte)17);
    for (int a = 0; a < 300; a++) {
        answer[a] = (std::byte)66;
    }

    printResults(answer, genome, "Stress Test 6: ");

    delete genome;
}

// deletion of size 300 at beginning of size 100000 genome
// debug doesn't print entire genome since it's very large
// user can modify test to print what they want when debug is set
template <class genomeName>
void stressTest7(bool debug) {
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

    std::vector<std::byte> answer(99700, (std::byte)17);

    printResults(answer, genome, "Stress Test 7: ");

    delete genome;
}

// point, copy, insert, and delete mutations on size 1 million genome
// debug doesn't print entire genome since it's very large
// user can modify test to print what they want when debug is set
template<class genomeName>
void stressTest8(bool debug) {
    AbstractGenome* genome = new genomeName(1000000); 

    // sets all 1 million values in genome to 10 initially
    for (int i = 0; i < genome->size(); i++) {
        GN::genomeWrite<std::byte>(genome, i, (std::byte)10);
    }

    genome->remove(0, 200); // delete first 200 sites, size becomes 999800

    // insert 300 sites of value 15 at position 0, size becomes 1000100
    std::vector<std::byte> insertionVals(300, (std::byte)15);
    genome->insert(0, insertionVals);

    // change position 5 to have value 30
    genome->overwrite(5, std::vector<std::byte>({(std::byte)30}));

    // set positions 301 to 351 to value 15 by copy mutation
    std::vector<std::byte> copyVals(50);
    for (int copyPos = 200; copyPos < 250; copyPos++)
        copyVals[copyPos-200] = GN::genomeRead<std::byte>(genome, copyPos);
    genome->overwrite(301, copyVals);

    // positions 0 to 4 have value 15
    // then position 5 has value 30
    // then positions 6 to 299 have value 15
    // then position 300 has value 10
    // then positions 301 to 350 have value 15
    // then positions 351 to end of genome have values 10
    // final genome size = 1000100
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

    printResults(answer, genome, "Stress Test 8: ");

    delete genome;
}

// runs all point and copy mutation tests
template <class genomeName>
void runOverwriteTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING OVERWRITE TESTS: " << std::endl;
    pointTest1<genomeName>(debug);
    copyTest1<genomeName>(debug);
}

// runs all delete/remove tests
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
}

// runs all tests which test overwrite+insert+delete together
template <class genomeName>
void runAllMutationsTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING ALL-MUTATIONS TESTS: " << std::endl;
    allMutationsTest1<genomeName>(debug);
    allMutationsTest2<genomeName>(debug);
    allMutationsTest3<genomeName>(debug);
}

// runs all stress tests
template <class genomeName>
void runAllStressTests(bool debug) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "RUNNING STRESS TESTS: " << std::endl;
    stressTest1<genomeName>(debug);
    stressTest2<genomeName>(debug);
    stressTest3<genomeName>(debug);
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
    runDeleteTests<genomeName>(debug);
    runInsertTests<genomeName>(debug);
    runAllMutationsTests<genomeName>(debug);
    runAllStressTests<genomeName>(debug);
}