#ifdef DEBUG
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

void print_map(const std::map<int, std::pair<int, bool>>& r_map);
void mutation_point(int ind, int number_of_sites, std::map<int, std::pair<int, bool>>& r_map);
void mutation_delete(int ind, int shift, std::map<int, std::pair<int, bool>>& r_map);
void mutation_insert(int ind, const std::vector<int>& ins_vals, std::map<int, std::pair<int, bool>>& r_map, std::unordered_map<int, std::vector<int>>& insertions);
int get_ind(int ind_curr, const std::map<int, std::pair<int, bool>>& r_map);

std::vector<int> offspring_recon(const std::vector<int>& genome_orig,
                                const std::map<int, std::pair<int, bool>>& change_log_map);


#ifdef DEBUG
TEST_CASE("Empty parent genome") {
    std::vector<int> genome{}; // how is it different from std::vector<int> genome;
    std::map<int, std::pair<int, bool>> r_map{};
    std::unordered_map<int, std::vector<int>> insertions;

    SECTION("Delete from empty genome") {
        // map and index calculations do not change
        // TODO add tests for when accessing parent genome using indexes (possible out of bound)
        // TODO add tests for when reconstructing offspring genome using indexes (possible out of bound)
        mutation_delete(3, 1, r_map);
        REQUIRE(get_ind(2, r_map) == 2);
        REQUIRE(get_ind(3, r_map) == (3 + 1));
        REQUIRE(get_ind(4, r_map) == (4 + 1));
    }

    SECTION("Insert into empty genome") {
        mutation_insert(4, std::vector<int>{1, 2}, r_map, insertions);
        REQUIRE(get_ind(3, r_map) == 3);
        REQUIRE(get_ind(4, r_map) == -1);
        REQUIRE(get_ind(5, r_map) == -1);
        REQUIRE(get_ind(6, r_map) == (6 - 2));
    }
}


TEST_CASE("Edge cases")
{
    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51}; // size() is 11
    std::map<int, std::pair<int, bool>> r_map;
    std::unordered_map<int, std::vector<int>> insertions;

    SECTION("Delete at the beginning") {
        mutation_delete(0, 1, r_map);
        REQUIRE(get_ind(0, r_map) == (0 + 1));
        REQUIRE(get_ind(1, r_map) == (1 + 1));
        REQUIRE(get_ind(7, r_map) == (7 + 1));
    }

    SECTION("Delete at the end") {
        mutation_delete(genome.size() - 1, 1, r_map); // Implicit conversion loses integer precision: 'unsigned long' to 'int'?
        REQUIRE(get_ind(0, r_map) == 0);
        REQUIRE(get_ind(5, r_map) == 5);
        REQUIRE(get_ind(genome.size() - 2, r_map) == ((genome.size() - 2)));
        REQUIRE(get_ind(genome.size() - 1, r_map) == ((genome.size() - 1) + 1)); // this is outside of the offspring genome
                                                                                 // TODO add test for accessing parent genome and reconstructing offspring genome
    }

    SECTION("Insert at the beginning") {
        mutation_insert(0, std::vector<int>{1, 2}, r_map, insertions);
        REQUIRE(r_map.size() == 2);
        REQUIRE(r_map.begin()->first == 0);
        REQUIRE(r_map.begin()->second.first == 0);
        REQUIRE(r_map.begin()->second.second == true);
        REQUIRE(std::next(r_map.begin(), 1)->first == 2); // key of the second element
        REQUIRE(std::next(r_map.begin(), 1)->second.first == 2); // value of the second element
        REQUIRE(std::next(r_map.begin(), 1)->second.second == false);
        REQUIRE(get_ind(0, r_map) == -1);
        REQUIRE(get_ind(1, r_map) == -1);
        REQUIRE(get_ind(2, r_map) == (2 - 2));
        REQUIRE(get_ind(5, r_map) == (5 - 2));
    }

    SECTION("Insert at the end") {
        mutation_insert(genome.size() - 1, std::vector<int>{1, 2}, r_map, insertions);
        REQUIRE(get_ind(0, r_map) == 0);
        REQUIRE(get_ind(1, r_map) == 1);
        REQUIRE(get_ind(10, r_map) == -1);
        REQUIRE(get_ind(11, r_map) == -1);
        REQUIRE(get_ind(15, r_map) == (15 - 2));
    }

    SECTION("Delete all sites") {
        // TODO
    }
}

SCENARIO("Change log can be used to reconstruct offspring genome from original genome") {

    GIVEN("Original genome and empty change log") {

        std::vector<int> genome_orig{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
        std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
        std::unordered_map<int, std::vector<int>> insertions;

        //REQUIRE();

        WHEN("del 1 site at ind 3") {

            mutation_delete(3, 1, change_log);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {3, {-1, false}}
                };
                std::vector<int> expected_indx{0, 1, 2, 4, 5, 6, 7, 8, 9, 10};
                std::vector<int> expected_offspring{21, 35, 43, 12, 15, 19, 28, 39, 45, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

        WHEN("del 1 site at ind 4 && ins 1 site at ind 3") {

            mutation_delete(3, 1, change_log);
            mutation_insert(4, std::vector<int>{1}, change_log, insertions);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {3, {-1, false}},
                    {4, {0, true}},
                    {5, {0, false}}
                };
                std::vector<int> expected_indx{0, 1, 2, 4, -1, 5, 6, 7, 8, 9, 10};
                std::vector<int> expected_offspring{21, 35, 43, 12, -1, 15, 19, 28, 39, 45, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

        WHEN("del 1 site at ind 4 && ins 1 site at ind 3 && del 2 sites at ind 6") {

            mutation_delete(3, 1, change_log);
            mutation_insert(4, std::vector<int>{1}, change_log, insertions);
            mutation_delete(6, 2, change_log);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {3, {-1, false}},
                    {4, {0, true}},
                    {5, {0, false}},
                    {6, {-2, false}}
                };
                std::vector<int> expected_indx{0, 1, 2, 4, -1, 5, 8, 9, 10};
                std::vector<int> expected_offspring{21, 35, 43, 12, -1, 15, 39, 45, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

        WHEN("del 1 site at ind 4 && ins 1 site at ind 3 && del 2 sites at ind 6 && ins 3 sites at ind 5") {

            mutation_delete(3, 1, change_log);
            mutation_insert(4, std::vector<int>{1}, change_log, insertions);
            mutation_delete(6, 2, change_log);
            mutation_insert(5, std::vector<int>{1, 2, 3}, change_log, insertions);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {3, {-1, false}},
                    {4, {0, true}},
                    /*{5, {0, true}},*/ // 4 and 5 overlap
                    {8, {3, false}},
                    {9, {1, false}}
                };
                std::vector<int> expected_indx{0, 1, 2, 4, -1, -1, -1, -1, 5, 8, 9, 10};
                std::vector<int> expected_offspring{21, 35, 43, 12, -1, -1, -1, -1, 15, 39, 45, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

        WHEN("del 1 site at ind 4 && ins 1 site at ind 3 && del 2 sites at ind 6 "
             "&& ins 3 sites at ind 5 && del 3 sites at ind 6") {

            mutation_delete(3, 1, change_log);
            mutation_insert(4, std::vector<int>{1}, change_log, insertions);
            mutation_delete(6, 2, change_log);
            mutation_insert(5, std::vector<int>{1, 2, 3}, change_log, insertions);
            mutation_delete(6, 3, change_log);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {3, {-1, false}},
                    {4, {0, true}},
                    /*{5, {0, true}},*/ // 4 and 5 overlap
                    {6, {-2, false}}
                };
                std::vector<int> expected_indx{0, 1, 2, 4, -1, -1, 8, 9, 10};
                std::vector<int> expected_offspring{21, 35, 43, 12, -1, -1, 39, 45, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

        WHEN("del 1 site at ind 4 && ins 1 site at ind 3 && del 2 sites at ind 6 "
             "&& ins 3 sites at ind 5 && del 3 sites at ind 6 && del 2 sites at ind 6") {

            mutation_delete(3, 1, change_log);
            mutation_insert(4, std::vector<int>{1}, change_log, insertions);
            mutation_delete(6, 2, change_log);
            mutation_insert(5, std::vector<int>{1, 2, 3}, change_log, insertions);
            mutation_delete(6, 3, change_log);
            mutation_delete(6, 2, change_log);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {3, {-1, false}},
                    {4, {0, true}},
                    /*{5, {0, true}},*/ // 4 and 5 overlap
                    {6, {-4, false}}
                };
                std::vector<int> expected_indx{0, 1, 2, 4, -1, -1, 10};
                std::vector<int> expected_offspring{21, 35, 43, 12, -1, -1, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

        WHEN("del 1 site at ind 4 && ins 1 site at ind 3 && del 2 sites at ind 6 "
             "&& ins 3 sites at ind 5 && del 3 sites at ind 6 && del 2 sites at ind 6"
             "&& ins 3 sites at ind 0") {

            mutation_delete(3, 1, change_log);
            mutation_insert(4, std::vector<int>{1}, change_log, insertions);
            mutation_delete(6, 2, change_log);
            mutation_insert(5, std::vector<int>{1, 2, 3}, change_log, insertions);
            mutation_delete(6, 3, change_log);
            mutation_delete(6, 2, change_log);
            mutation_insert(0, std::vector<int>{1, 2, 3}, change_log, insertions);

            THEN("both offspring genome and change log are updated") {

                std::map<int, std::pair<int, bool>> expected_log{
                    /*{0, {0, false}},*/
                    {0, {0, true}},
                    {3, {3, false}},
                    {6, {2, false}},
                    {7, {0, true}},
                    /*{8, {0, true}},*/ // 7 and 8 overlap
                    {9, {-1, false}}
                };
                std::vector<int> expected_indx{-1, -1, -1, 0, 1, 2, 4, -1, -1, 10};
                std::vector<int> expected_offspring{-1, -1, -1, 21, 35, 43, 12, -1, -1, 51};

                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                // Iterate over two maps simultaneously
                for (auto it_ch_b = change_log.cbegin(), it_ch_e = change_log.cend(),
                     it_ecl_b = expected_log.cbegin(), it_ecl_e = expected_log.cend();
                     it_ch_b != it_ch_e /*|| it_ecl_b != it_ecl_e*/;) {// same size

                    CHECK(it_ch_b->first == it_ecl_b->first);
                    CHECK(it_ch_b->second.first == it_ecl_b->second.first);
                    CHECK(it_ch_b->second.second == it_ecl_b->second.second);
                    ++it_ch_b;
                    ++it_ecl_b;
                }
                REQUIRE(change_log == expected_log);

                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }

                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

    }
}

SCENARIO("Insert within insert") {
    
    GIVEN("Original genome and empty change log") {
        
        std::vector<int> genome_orig{21, 35, 43, 84, 12};
        std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
        std::unordered_map<int, std::vector<int>> insertions;
                
        WHEN("ins 4 sites at ind 2") {
            
            mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
            
            THEN("both offspring genome and change log are updated") {
                
                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {2, {0, true}},
                    {6, {4, false}}
                };
                std::vector<int> expected_indx{0, 1, -1, -1, -1, -1, 2, 3, 4};
                std::vector<int> expected_offspring{21, 35, -1, -1, -1, -1, 43, 84, 12};
                //std::vector<int> expected_offspring{21, 35, 1, 2, 3, 4, 43, 84, 12};
                
                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                REQUIRE(change_log == expected_log);
                
                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }
                
                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }
        
        WHEN("ins 4 sites at ind 2 && ins 2 sites at ind 3") {
            
            mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
            mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
            
            THEN("both offspring genome and change log are updated") {
                
                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {2, {0, true}},
                    /*{3, {0, true}},*/
                    {8, {6, false}}
                };
                std::vector<int> expected_indx{0, 1, -1, -1, -1, -1, -1, -1, 2, 3, 4};
                std::vector<int> expected_offspring{21, 35, -1, -1, -1, -1, -1, -1, 43, 84, 12};
                //std::vector<int> expected_offspring{21, 35, 1, 5, 6, 2, 3, 4, 43, 84, 12};
                
                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                for (auto it_ch_b = change_log.cbegin(), it_ch_e = change_log.cend(),
                     it_ecl_b = expected_log.cbegin(), it_ecl_e = expected_log.cend();
                     it_ch_b != it_ch_e /*|| it_ecl_b != it_ecl_e*/;) {// same size
                    
                    CHECK(it_ch_b->first == it_ecl_b->first);
                    CHECK(it_ch_b->second.first == it_ecl_b->second.first);
                    CHECK(it_ch_b->second.second == it_ecl_b->second.second);
                    ++it_ch_b;
                    ++it_ecl_b;
                }
                REQUIRE(change_log == expected_log);
                
                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }
                
                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }
        
        WHEN("ins 4 sites at ind 2 && ins 2 sites at ind 3 && del 3 sites at ind 1") {
            
            mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
            mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
            mutation_delete(1, 3, change_log);
            
            THEN("both offspring genome and change log are updated") {
                
                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {1, {0, true}},
                    {5, {3, false}}
                };
                std::vector<int> expected_indx{0, -1, -1, -1, -1, 2, 3, 4};
                std::vector<int> expected_offspring{21, -1, -1, -1, -1, 43, 84, 12};
                //std::vector<int> expected_offspring{21, 6, 2, 3, 4, 43, 84, 12};
                
                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                for (auto it_ch_b = change_log.cbegin(), it_ch_e = change_log.cend(),
                     it_ecl_b = expected_log.cbegin(), it_ecl_e = expected_log.cend();
                     it_ch_b != it_ch_e /*|| it_ecl_b != it_ecl_e*/;) {// same size
                    
                    CHECK(it_ch_b->first == it_ecl_b->first);
                    CHECK(it_ch_b->second.first == it_ecl_b->second.first);
                    CHECK(it_ch_b->second.second == it_ecl_b->second.second);
                    ++it_ch_b;
                    ++it_ecl_b;
                }
                REQUIRE(change_log == expected_log);
                
                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }
                
                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }
        
        WHEN("ins 4 sites at ind 2 && ins 2 sites at ind 3 && del 3 sites at ind 1 && del 4 sites at ind 3") {
            
            mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
            mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
            mutation_delete(1, 3, change_log);
            mutation_delete(3, 4, change_log);
            
            THEN("both offspring genome and change log are updated") {
                
                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {1, {0, true}},
                    {3, {-1, false}}
                };
                std::vector<int> expected_indx{0, -1, -1, 4};
                std::vector<int> expected_offspring{21, -1, -1, 12};
                //std::vector<int> expected_offspring{21, 6, 2, 3, 4, 43, 84, 12};
                
                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                for (auto it_ch_b = change_log.cbegin(), it_ch_e = change_log.cend(),
                     it_ecl_b = expected_log.cbegin(), it_ecl_e = expected_log.cend();
                     it_ch_b != it_ch_e /*|| it_ecl_b != it_ecl_e*/;) {// same size
                    
                    CHECK(it_ch_b->first == it_ecl_b->first);
                    CHECK(it_ch_b->second.first == it_ecl_b->second.first);
                    CHECK(it_ch_b->second.second == it_ecl_b->second.second);
                    ++it_ch_b;
                    ++it_ecl_b;
                }
                REQUIRE(change_log == expected_log);
                
                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }
                
                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }
        
        WHEN("ins 4 sites at ind 2 && ins 2 sites at ind 3 && del 3 sites at ind 1 && del 3 sites at ind 1") {
            
            mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
            mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
            mutation_delete(1, 3, change_log);
            mutation_delete(1, 3, change_log);
            
            THEN("both offspring genome and change log are updated") {
                
                std::map<int, std::pair<int, bool>> expected_log{
                    {0, {0, false}},
                    {1, {0, true}},
                    {2, {0, false}}
                };
                std::vector<int> expected_indx{0, -1, 2, 3, 4};
                std::vector<int> expected_offspring{21, -1, 43, 84, 12};
                //std::vector<int> expected_offspring{21, 4, 43, 84, 12};
                
                // Check change log:
                // to get specific error, need to iterate through both std::maps
                // this would work for vectors, but not maps yet
                REQUIRE(change_log.size() == expected_log.size());
                for (auto it_ch_b = change_log.cbegin(), it_ch_e = change_log.cend(),
                     it_ecl_b = expected_log.cbegin(), it_ecl_e = expected_log.cend();
                     it_ch_b != it_ch_e /*|| it_ecl_b != it_ecl_e*/;) {// same size
                    
                    CHECK(it_ch_b->first == it_ecl_b->first);
                    CHECK(it_ch_b->second.first == it_ecl_b->second.first);
                    CHECK(it_ch_b->second.second == it_ecl_b->second.second);
                    ++it_ch_b;
                    ++it_ecl_b;
                }
                REQUIRE(change_log == expected_log);
                
                // Check indexes:
                for (int i = 0; i < expected_indx.size(); ++i) {
                    REQUIRE(get_ind(i, change_log) == expected_indx[i]);
                }
                
                // Check reconstructed offspring genome:
                CHECK(offspring_recon(genome_orig, change_log) == expected_offspring);
            }
        }

    }
}



#endif

