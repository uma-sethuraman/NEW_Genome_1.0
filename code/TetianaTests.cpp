#include "catch.hpp"

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "TestGenome.h"
#include "TetianaGenome.h"


SCENARIO("Test genome mutations") {
    
    
    GIVEN("Empty genome") {

        AbstractGenome* testGenome = new TetianaGenome(0);

        WHEN("Inserting sites at the beginning once") {

            testGenome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });

            THEN("Possible to reconstruct offspring genome") {

                auto offspring_data = testGenome->data(0, 0);

                CHECK(*(offspring_data + 0) == (std::byte)1);
                CHECK(*(offspring_data + 1) == (std::byte)2);
                CHECK(*(offspring_data + 2) == (std::byte)3);
            }
        }
        WHEN("Inserting sites at the beginning twice") {

            testGenome->insert(0, std::vector<std::byte>{ (std::byte)1, (std::byte)2, (std::byte)3 });
            testGenome->insert(0, std::vector<std::byte>{ (std::byte)4, (std::byte)5, (std::byte)6 });

            THEN("Possible to reconstruct offspring genome") {

                auto offspring_data = testGenome->data(0, 0);

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
                
                //CHECK(myGenome->size() == testGenome->size());

                std::byte* my_offspring_data = myGenome->data(0, 0);
                std::byte* test_offspring_data = testGenome->data(0, 0);

                for(int i = 0; i < myGenome->size(); i++) {
                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
                }

            }
        }
    }
        
        
    GIVEN("Non-empty genome of size 11") {
        
        size_t g_size = 11;

        AbstractGenome* myGenome = new TetianaGenome(g_size);
        AbstractGenome* testGenome = new TestGenome(g_size);
        
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
            myGenome->insert(2, segment1);
            testGenome->insert(2, segment1);
            
            
            //etc
            
            THEN("Possible to reconstruct offspring genome") {
                
                std::byte* my_offspring_data = myGenome->data(0, 0);
                std::byte* test_offspring_data = testGenome->data(0, 0);
                
                for(int i = 0; i < myGenome->size(); i++) {
                    CHECK(*(my_offspring_data + i) == *(test_offspring_data + i));
                }
                
            }
        }
    }
}
