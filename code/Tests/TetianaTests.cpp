#include "Tools/catch.hpp"

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "Genomes/TestGenome.h"
#include "Genomes/TetianaGenome.h"


SCENARIO("Test genome mutations ???") {
    
    GIVEN("Empty genome") {
        
        AbstractGenome* myGenome = new TetianaGenome(0);
        
        WHEN("Inserting sites at the beginning once") {
            
            myGenome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
            
            THEN("Possible to reconstruct offspring genome") {
                
                auto offspring_data = myGenome->data(0, 0);
                
//                std::cout << (int)(*(offspring_data + 0)) << ", " << (int)((std::byte)1) << std::endl;
//                std::cout << (int)(*(offspring_data + 1)) << ", " << (int)((std::byte)2) << std::endl;
//                std::cout << (int)(*(offspring_data + 2)) << ", " << (int)((std::byte)3) << std::endl;
                
                CHECK(*(offspring_data + 0) == (std::byte)1);
                CHECK(*(offspring_data + 1) == (std::byte)2);
                CHECK(*(offspring_data + 2) == (std::byte)3);
                
//                std::cout << "==========" << std::endl;
//                std::cout << (int)(*(offspring_data + 0)) << ", " << (int)((std::byte)1) << std::endl;
//                std::cout << (int)(*(offspring_data + 1)) << ", " << (int)((std::byte)2) << std::endl;
//                std::cout << (int)(*(offspring_data + 2)) << ", " << (int)((std::byte)3) << std::endl;
            }
        }
    }
}

AbstractGenome* myGenome = new TetianaGenome(0);
AbstractGenome* testGenome = new TestGenome(0);


SCENARIO("Test Remove") {
//    GIVEN("Empty genome") {
//        myGenome->resize(0);
//        WHEN("remove(0, 5)") {
//            myGenome->remove(0, 5);
//            THEN("Genome still empty") {
//                CHECK(myGenome->size() == 0);
//            }
//        }
//        WHEN("remove(5, 5)") {
//            myGenome->remove(5, 5);
//            THEN("Genome still empty") {
//                CHECK(myGenome->size() == 0);
//            }
//        }
//    }

    GIVEN("Non-empty genome of size 5") {

        size_t g_size = 5;

        //AbstractGenome* myGenome = new TetianaGenome(g_size);
        //AbstractGenome* testGenome = new TestGenome(g_size);

        myGenome->resize(g_size);
        testGenome->resize(g_size);

        GN::genomeWrite<std::byte>(myGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(myGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(myGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(myGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(myGenome, 4, (std::byte)12);

        GN::genomeWrite<std::byte>(testGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(testGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(testGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(testGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(testGenome, 4, (std::byte)12);


        WHEN("Remove at the beginning: remove(0, 2)") {

            myGenome->remove(0, 2);
            testGenome->remove(0, 2);

            THEN("Reconstruct offspring genome") {
                CHECK(myGenome->size() == testGenome->size());

                std::byte* my_offspring_data = myGenome->data(0, 0);
                std::byte* test_offspring_data = testGenome->data(0, 0);

                for(int i = 0; i < myGenome->size(); i++) {
                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
                }
            }
        }

//        WHEN("Remove at the end: remove(myGenome->size(), 2)") {
//
//            // Should this be allowed?
//
//            myGenome->remove(myGenome->size(), 2);
//
//            THEN("Nothing changes") {
//
//                CHECK(myGenome->size() == testGenome->size());
//
//                std::byte* my_offspring_data = myGenome->data(0, 0);
//                std::byte* test_offspring_data = testGenome->data(0, 0);
//
//                for(int i = 0; i < myGenome->size(); i++) {
//                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
//                }
//            }
//        }

//        WHEN("Remove at last index: remove(myGenome->size() - 1, 1)") {
//
//            myGenome->remove(myGenome->size() - 1, 1);
//            testGenome->remove(testGenome->size() - 1, 1);
//
//            THEN("Nothing changes") {
//
//                CHECK(myGenome->size() == testGenome->size());
//
//                std::byte* my_offspring_data = myGenome->data(0, 0);
//                std::byte* test_offspring_data = testGenome->data(0, 0);
//
//                for(int i = 0; i < myGenome->size(); i++) {
//                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
//                }
//            }
//        }
    }


    GIVEN("Non-empty genome of size 11") {

        size_t g_size = 11;

        //        AbstractGenome* myGenome = new TetianaGenome(g_size);
        //        AbstractGenome* testGenome = new TestGenome(g_size);

        myGenome->resize(g_size);
        testGenome->resize(g_size);

        GN::genomeWrite<std::byte>(myGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(myGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(myGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(myGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(myGenome, 4, (std::byte)12);
        GN::genomeWrite<std::byte>(myGenome, 5, (std::byte)15);
        GN::genomeWrite<std::byte>(myGenome, 6, (std::byte)19);
        GN::genomeWrite<std::byte>(myGenome, 7, (std::byte)28);
        GN::genomeWrite<std::byte>(myGenome, 8, (std::byte)39);
        GN::genomeWrite<std::byte>(myGenome, 9, (std::byte)45);
        GN::genomeWrite<std::byte>(myGenome, 10, (std::byte)51);

        GN::genomeWrite<std::byte>(testGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(testGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(testGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(testGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(testGenome, 4, (std::byte)12);
        GN::genomeWrite<std::byte>(testGenome, 5, (std::byte)15);
        GN::genomeWrite<std::byte>(testGenome, 6, (std::byte)19);
        GN::genomeWrite<std::byte>(testGenome, 7, (std::byte)28);
        GN::genomeWrite<std::byte>(testGenome, 8, (std::byte)39);
        GN::genomeWrite<std::byte>(testGenome, 9, (std::byte)45);
        GN::genomeWrite<std::byte>(testGenome, 10, (std::byte)51);

        WHEN("Delete 1 site at index 3 &&"
             "Insert {1} at index 4 &&") {

            myGenome->remove(3, 1);
            testGenome->remove(3, 1);

            std::vector<std::byte> segment1{
                (std::byte)1
            };
            myGenome->insert(4, segment1);
            testGenome->insert(4, segment1);

            myGenome->remove(6, 2);
            testGenome->remove(6, 2);

            std::vector<std::byte> segment2{
                (std::byte)2,
                (std::byte)3,
                (std::byte)4
            };
            myGenome->insert(5, segment2);
            testGenome->insert(5, segment2);

            myGenome->remove(6, 3);
            testGenome->remove(6, 3);

            myGenome->remove(6, 2);
            testGenome->remove(6, 2);

            std::vector<std::byte> segment3{
                (std::byte)5,
                (std::byte)6,
                (std::byte)7
            };
            myGenome->insert(0, segment3);
            testGenome->insert(0, segment3);

            //myGenome->remove(0, 15);
            //testGenome->remove(0, 15);


            THEN("Possible to reconstruct offspring genome") {

                CHECK(myGenome->size() == testGenome->size());

                std::byte* my_offspring_data = myGenome->data(0, 0);
                std::byte* test_offspring_data = testGenome->data(0, 0);

                for(int i = 0; i < myGenome->size(); i++) {
                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
                }

            }
        }
    }
}



SCENARIO("Test genome mutations") {


    GIVEN("Empty genome") {

        AbstractGenome* myGenome = new TetianaGenome(0);

        WHEN("Inserting sites at the beginning once") {

            myGenome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });

            THEN("Possible to reconstruct offspring genome") {

                auto offspring_data = myGenome->data(0, 0);

                CHECK((int)(*(offspring_data + 0)) == 1);
                CHECK((int)(*(offspring_data + 1)) == 2);
                CHECK((int)(*(offspring_data + 2)) == 3);
            }
        }
        WHEN("Inserting sites at the beginning twice") {

            myGenome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
            myGenome->insert(0, std::vector<std::byte>{ (std::byte)4, (std::byte)5, (std::byte)6 });

            THEN("Possible to reconstruct offspring genome") {

                auto offspring_data = myGenome->data(0, 0);

                CHECK(*(offspring_data + 0) == (std::byte)4);
                CHECK(*(offspring_data + 1) == (std::byte)5);
                CHECK(*(offspring_data + 2) == (std::byte)6);
                CHECK(*(offspring_data + 3) == (std::byte)1);
                CHECK(*(offspring_data + 4) == (std::byte)2);
                CHECK(*(offspring_data + 5) == (std::byte)3);
            }
        }
    }

    GIVEN("Non-empty genome of size 5") {

        size_t g_size = 5;

        AbstractGenome* myGenome = new TetianaGenome(g_size);
        AbstractGenome* testGenome = new TestGenome(g_size);

        //myGenome->resize(g_size);
        //testGenome->resize(g_size);

        GN::genomeWrite<std::byte>(myGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(myGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(myGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(myGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(myGenome, 4, (std::byte)12);

        GN::genomeWrite<std::byte>(testGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(testGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(testGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(testGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(testGenome, 4, (std::byte)12);


        WHEN("Insert {1, 2, 3, 4} at index 2 && Insert {5, 6} at index 3 && Remove 3 sites at index 1 && Remove 4 sites at index 3") {

            std::vector<std::byte> segment1{
                (std::byte)1,
                (std::byte)2,
                (std::byte)3,
                (std::byte)4
            };
            myGenome->insert(2, segment1);
            testGenome->insert(2, segment1);

            std::vector<std::byte> segment2{
                (std::byte)5,
                (std::byte)6
            };
            myGenome->insert(2, segment2);
            testGenome->insert(2, segment2);

            myGenome->remove(1, 3);
            testGenome->remove(1, 3);

            myGenome->remove(3, 4);
            testGenome->remove(3, 4);

            THEN("Possible to reconstruct offspring genome") {

                CHECK(myGenome->size() == testGenome->size());

                std::byte* my_offspring_data = myGenome->data(0, 0);
                std::byte* test_offspring_data = testGenome->data(0, 0);

                for(int i = 0; i < myGenome->size(); i++) {
                    CHECK((int)(*(my_offspring_data + i)) == (int)(*(test_offspring_data + i)));
                }
            }
        }
    }


    GIVEN("Non-empty genome of size 11") {

        size_t g_size = 11;

        AbstractGenome* myGenome = new TetianaGenome(g_size);
        AbstractGenome* testGenome = new TestGenome(g_size);

        //myGenome->resize(g_size);
        //testGenome->resize(g_size);

        GN::genomeWrite<std::byte>(myGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(myGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(myGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(myGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(myGenome, 4, (std::byte)12);
        GN::genomeWrite<std::byte>(myGenome, 5, (std::byte)15);
        GN::genomeWrite<std::byte>(myGenome, 6, (std::byte)19);
        GN::genomeWrite<std::byte>(myGenome, 7, (std::byte)28);
        GN::genomeWrite<std::byte>(myGenome, 8, (std::byte)39);
        GN::genomeWrite<std::byte>(myGenome, 9, (std::byte)45);
        GN::genomeWrite<std::byte>(myGenome, 10, (std::byte)51);

        GN::genomeWrite<std::byte>(testGenome, 0, (std::byte)21);
        GN::genomeWrite<std::byte>(testGenome, 1, (std::byte)35);
        GN::genomeWrite<std::byte>(testGenome, 2, (std::byte)43);
        GN::genomeWrite<std::byte>(testGenome, 3, (std::byte)84);
        GN::genomeWrite<std::byte>(testGenome, 4, (std::byte)12);
        GN::genomeWrite<std::byte>(testGenome, 5, (std::byte)15);
        GN::genomeWrite<std::byte>(testGenome, 6, (std::byte)19);
        GN::genomeWrite<std::byte>(testGenome, 7, (std::byte)28);
        GN::genomeWrite<std::byte>(testGenome, 8, (std::byte)39);
        GN::genomeWrite<std::byte>(testGenome, 9, (std::byte)45);
        GN::genomeWrite<std::byte>(testGenome, 10, (std::byte)51);

        WHEN("Delete 1 site at index 3 && "
             "Insert {1} at index 4 && ") {

            myGenome->remove(3, 1);
            testGenome->remove(3, 1);

            std::vector<std::byte> segment1{
                (std::byte)1
            };
            myGenome->insert(4, segment1);
            testGenome->insert(4, segment1);

            myGenome->remove(6, 2);
            testGenome->remove(6, 2);

            std::vector<std::byte> segment2{
                (std::byte)2,
                (std::byte)3,
                (std::byte)4
            };
            myGenome->insert(5, segment2);
            testGenome->insert(5, segment2);

            myGenome->remove(6, 3);
            testGenome->remove(6, 3);

            myGenome->remove(6, 2);
            testGenome->remove(6, 2);

            std::vector<std::byte> segment3{
                (std::byte)5,
                (std::byte)6,
                (std::byte)7
            };
            myGenome->insert(0, segment3);
            testGenome->insert(0, segment3);

            //myGenome->remove(0, 15);
            //testGenome->remove(0, 15);


            THEN("Possible to reconstruct offspring genome") {

                CHECK(myGenome->size() == testGenome->size());

                std::byte* my_offspring_data = myGenome->data(0, 0);
                std::byte* test_offspring_data = testGenome->data(0, 0);

                for(int i = 0; i < myGenome->size(); i++) {
                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
                }
            }

        }
    }
}
