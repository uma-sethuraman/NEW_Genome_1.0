#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "TetianaGenome.h"



SCENARIO("Test genome mutations") {
    
    AbstractGenome* testGenome = new TetianaGenome(0);
    
    GIVEN("Empty genome") {
        
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
}


