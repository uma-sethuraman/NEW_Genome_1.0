

#ifdef DEBUG
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif

#include <iostream>
#include <map>
#include <vector>
//#include <algorithm>
//#include <memory>

void print_map(const std::map<int, std::pair<int, bool>>& r_map) {
    std::cout << "Map:" << std::endl;
    for (auto it = r_map.begin(); it != r_map.end(); ++it) {
        std::cout << it->first << " : " <<
        it->second.first << " : " << it->second.second << std::endl;
    }
    
}

void mutation_delete(int ind, int shift, std::map<int, std::pair<int, bool>>& r_map) {
    //std::cout << "in mutation_delete" << std::endl;
    if (r_map.lower_bound(ind) == r_map.end()) {
        // nothing equal or after ind
        r_map[ind] = std::make_pair(-shift, false);
    } else {
        // TODO if key >= ind exists
    }
    
}

void mutation_insert(int ind, int shift, std::map<int, std::pair<int, bool>>& r_map) {
    //std::cout << "in mutation_insert" << std::endl;
    
    /*
     Insert:
     - add index, # of sites to shift and true
     Cases:
     - ind > largest key in map => just insert ind and val = 0
     - ind <= largest key in map =>
     - add ind with val = 0
     - every key >= ind has to be updated
     
     */
    
    //auto next_key_it = r_map.upper_bound(ind); // == r_map.end() if there is no value > ind in the map
    auto next_key_it = r_map.lower_bound(ind); // == r_map.end() if there is no value >= ind in the map
                                               // I think lower_bound should be used, because the situation when ind is already in the map
                                               //  requires separate processing
    
    //std::cout << "ind: " << ind << std::endl;
    //if (next_key_it != r_map.end())
    //    std::cout << "next_key_it->first: " << next_key_it->first << std::endl;
    //else
    //    std::cout << "next_key_it == r_map.end()" << std::endl;
    
    if (next_key_it == r_map.end()) { // ind > largest key in map
                                      // there is no key in the map that is either == or > than ind
                                      // => add the ind and val = 0; then add ind + shift and val = shift
                                      // this meant that for ind ... ind + shift the val = 0 (inserted sites)
                                      // starting from ind_shift the val =  + shift (take previous shift and add new one)
        
        // Insert current ind
        auto inserted = r_map.insert(std::make_pair(ind, std::make_pair(0, true))); // emplace_hint instead of insert?
        // Move the ones after it
        if (inserted.first == r_map.begin()) // map was empty just before the previous line
            r_map.insert(std::make_pair(ind + shift, std::make_pair(shift, false)));
        else
            r_map.insert(std::make_pair(ind + shift, std::make_pair((--inserted.first)->second.first + shift, false)));
        
    } else { // ind <= largest key in map
             // whether ind == next_key_it or < next_key_it
             // go through keys >= next_key_it and add shift
             // important to start with next_key_it, because if next_key_it == ind
             // we will lose the info at next_key_it when inserting (ind, 0)
        //for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != r_map.end(); ++it) {
        //    it->first += shift;
        //    it->second += shift;
        //}
        
        std::map<int, std::pair<int, bool>> temp_r_map;
        
        for (std::map<int, std::pair<int, bool>>::iterator it = r_map.begin(); it != next_key_it; ++it) {
            //std::cout << "inserting key: " << it->first << ", value: " << it->second.first << std::endl;
            temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != r_map.end(); ++it) {
            //std::cout << "shift: " << shift << std::endl;
            //std::cout << "inserting key: " << it->first + shift << ", value: " << it->second.first + shift << std::endl;
            temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first + shift, it->second.second)));
        }
            
        // the following can be optimized, as it repeats the code in "if"
        temp_r_map.insert(std::make_pair(ind, std::make_pair(0, true))); // emplace_hint instead of insert?
                                                                    //r_map.insert(std::make_pair(ind + shift, std::make_pair(shift, false))); // dont need this because I already shifter all prev keys
        r_map = temp_r_map;
        
    }
    
    //std::cout << "prev_it: " << prev_it->first << std::endl;
    //r_map[ind] = std::make_pair(0, true);
    //r_map[ind + shift] = std::make_pair(prev_it->second.first - shift, false);
}

int get_ind(int ind_curr, const std::map<int, std::pair<int, bool>>& r_map) {
    /*std::cout << "in get_ind" << std::endl;
    std::cout << "ind_curr: " << ind_curr << std::endl;
    std::cout << "r_map.size(): " << r_map.size() << std::endl;
    if (r_map.upper_bound(ind_curr) == r_map.end()) std::cout << "r_map.upper_bound(ind_curr) == r_map.end()" << std::endl;
    else std::cout << "r_map.upper_bound(ind_curr) != r_map.end()" << std::endl;
    if (r_map.upper_bound(ind_curr) == r_map.begin()) std::cout << "r_map.upper_bound(ind_curr) == r_map.begin()" << std::endl;
    else std::cout << "r_map.upper_bound(ind_curr) != r_map.begin()" << std::endl;
    */
    
    // upper_bound - returns an iterator pointing to the first element that is greater than key
    //auto it_curr = --r_map.upper_bound(ind_curr); // -- produces EXC_BAD_ACCESS when r_map.upper_bound(ind_curr) == r_map.begin()
    
    if (r_map.upper_bound(ind_curr) == r_map.begin())
        return ind_curr;
    else {
        auto it_curr = --r_map.upper_bound(ind_curr);
        if (it_curr->second.second == true) { // insertion
            return -1; // TODO replace with inserted vals
        }
        else {// deletion
            return ind_curr - it_curr->second.first;
        }
    }
}


#ifndef DEBUG
int main()
{
    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
    
    std::map<int, std::pair<int, bool>> r_map;
    
    mutation_delete(3, 1, r_map); // delete 1 site at index 2
    print_map(r_map);
    
    mutation_insert(4, 1, r_map); // insert 1 site at index 2
    print_map(r_map);
    
    mutation_delete(6, 2, r_map); // delete 2 site at index 6
    print_map(r_map);
    
    size_t new_size = genome.size() + r_map.rbegin()->second.first;
    std::cout << "size of new genome: " << new_size << std::endl;
    std::cout << "indexes:\n";
    for (int i = 0; i < new_size; ++i) {
        std::cout << get_ind(i, r_map) << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
#endif


#ifdef DEBUG
TEST_CASE("Empty parent genome")
{
    std::vector<int> genome{}; // how is it different from std::vector<int> genome;
    std::map<int, std::pair<int, bool>> r_map{};
    
    SECTION("Delete from empty genome") {
        // map and index calculations do not change
        // TODO add tests for when accessing parent genome using indexes (possible out of bound)
        // TODO add tests for when reconstructing ancestor genome using indexes (possible out of bound)
        mutation_delete(3, 1, r_map);
        REQUIRE(get_ind(2, r_map) == 2);
        REQUIRE(get_ind(3, r_map) == (3 + 1));
        REQUIRE(get_ind(4, r_map) == (4 + 1));
    }
    
    SECTION("Insert into empty genome") {
        mutation_insert(4, 2, r_map);
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
        REQUIRE(get_ind(genome.size() - 1, r_map) == ((genome.size() - 1) + 1)); // this is outside of the ancestor genome
        // TODO add test for accessing parent genome and reconstructing ancestor genome
    }
    
    SECTION("Insert at the beginning") {
        mutation_insert(0, 2, r_map);
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
        mutation_insert(genome.size() - 1, 2, r_map);
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

TEST_CASE("DeleteInsertSeparately")
{
    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
    std::map<int, std::pair<int, bool>> r_map;
    
    SECTION( "delete 1 site at index 3" ) {
        mutation_delete(3, 1, r_map);
        //print_map(r_map);
        REQUIRE(get_ind(2, r_map) == 2);
        REQUIRE(get_ind(3, r_map) == 4);
        REQUIRE(get_ind(4, r_map) == 5);
    }
    
    SECTION( "insert 1 site at index 4" ) {
        mutation_insert(4, 1, r_map);
        //print_map(r_map);
        REQUIRE(get_ind(3, r_map) == 3);
        REQUIRE(get_ind(4, r_map) == -1);
        REQUIRE(get_ind(5, r_map) == 4);
    }
}


TEST_CASE("DeleteInsertDelete")
{
    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
    std::map<int, std::pair<int, bool>> r_map;
    
    mutation_delete(3, 1, r_map);
    mutation_insert(4, 1, r_map);
    mutation_delete(6, 2, r_map);
    
    std::vector<int> inds{0, 1, 2, 4, -1, 5, 8, 9};
    for (int i = 0; i < inds.size(); ++i) { // i < genome.size()
        REQUIRE(get_ind(i, r_map) == inds[i]);
    }
}
#endif
