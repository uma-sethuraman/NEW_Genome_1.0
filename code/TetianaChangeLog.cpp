#include "catch.hpp"

#include <iostream>
#include <map>
#include <vector>
//#include <algorithm>
//#include <memory>

/*
class RangeMap {
public:
    std::map<int, int> r_map; // <mutation index, # if sites>

    // Constructor ?
    RangeMap(const int val) {
        r_map.insert(r_map.end(), std::make_pair(std::numeric_limits<int>::lowest(), val));
    }

    void assign(const int keyBegin, const int keyEnd, const int val) {
               
        if (!(keyBegin < keyEnd))
            return;
        
        std::map<int, int>::iterator itLowerBoundBegin = r_map.lower_bound(keyBegin);
        std::map<int, int>::iterator itLowerBoundBeginPrev = itLowerBoundBegin;
        if (itLowerBoundBegin != r_map.begin())
            itLowerBoundBeginPrev = std::prev(itLowerBoundBegin);
        
        std::map<int, int>::iterator itUpperBoundEnd = r_map.upper_bound(keyEnd);
        std::map<int, int>::iterator itUpperBoundEndPrev = std::prev(itUpperBoundEnd);
        
        int valEnd = itUpperBoundEndPrev->second;
        
        std::map<int, int>::iterator itInsertBegin = itLowerBoundBeginPrev;
        if (!(itLowerBoundBeginPrev->second == val))
            itInsertBegin = r_map.insert_or_assign(itLowerBoundBegin, keyBegin, val);
        
        std::map<int, int>::iterator itInsertEnd = r_map.erase(std::next(itInsertBegin), itUpperBoundEnd);
        if (!(val == valEnd))
            r_map.insert_or_assign(itInsertEnd, keyEnd, valEnd);
        
        // To test Canonicity
        for (auto iter = r_map.begin(); iter != r_map.end(); ++iter) {
            auto next = std::next(iter);
            if (next != r_map.end() && iter->second == next->second) {
                throw;
            }
        }
    }

    int getSize() {
        return r_map.size();
    }
    
    // look-up of the value associated with key
    const int& operator[](int const& key) const {
        return (--r_map.upper_bound(key))->second;
    }
};
*/




void print_map(const std::map<int, std::pair<int, bool>>& r_map) {
    std::cout << "Map:" << std::endl;
    for (auto it = r_map.begin(); it != r_map.end(); ++it) {
        std::cout << it->first << " : " <<
        it->second.first << " : " << it->second.second << std::endl;
    }
    
}

void mutation_delete(int ind, int shift, std::map<int, std::pair<int, bool>>& r_map) {
    std::cout << "in mutation_delete" << std::endl;
    if (r_map.lower_bound(ind) == r_map.end()) {
        // nothing equal or after ind
        r_map[ind] = std::make_pair(-shift, false);
    } else {
        // TODO if key >= ind exists
    }
    
}

void mutation_insert(int ind, int shift, std::map<int, std::pair<int, bool>>& r_map) {
    std::cout << "in mutation_insert" << std::endl;
    
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
    
    if (next_key_it == r_map.end()) { // ind > largest key in map
        // there is no key in the map that is either == or > than ind
        // => add the ind and val = 0; then add ind + shift and val = shift
        // this meant that for ind ... ind + shift the val = 0 (inserted sites)
        // starting from ind_shift the val = shift
        r_map.insert(std::make_pair(ind, std::make_pair(0, true))); // emplace_hint instead of insert?
        r_map.insert(std::make_pair(ind + shift, std::make_pair(shift, false)));
        
    } else { // ind <= largest key in map
        // whether ind == next_key_it or < next_key_it
        // go through keys >= next_key_it and add shift
        // important to start with next_key_it, because if next_key_it == ind
        // we will lose the info at next_key_it when inserting (ind, 0)
        for (auto it = next_key_it; it != r_map.end(); ++it) {
            it->first += shift;
            it->second += shift;
        }
        
        // the following can be optimized, as it repeats the code in "if"
        r_map.insert(std::make_pair(ind, std::make_pair(0, true))); // emplace_hint instead of insert?
        //r_map.insert(std::make_pair(ind + shift, std::make_pair(shift, false))); // dont need this because I already shifter all prev keys
    }
    
    //std::cout << "prev_it: " << prev_it->first << std::endl;
    //r_map[ind] = std::make_pair(0, true);
    //r_map[ind + shift] = std::make_pair(prev_it->second.first - shift, false);
}

int get_ind(int ind_curr, const std::map<int, std::pair<int, bool>>& r_map) {
    
    // upper_bound - returns an iterator pointing to the first element that is greater than key
    auto it_curr = --r_map.upper_bound(ind_curr);
    
    if (r_map.upper_bound(ind_curr) == r_map.begin())
        return ind_curr;
    else if (it_curr->second.second == true) // insertion
        return -1; // TODO replace with inserted vals
    else // deletion
        return ind_curr - it_curr->second.first;
}






int main()
{
    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
    
    std::map<int, std::pair<int, bool>> r_map;
    
    mutation_delete(3, 1, r_map); // delete 1 site at index 2
    print_map(r_map);
    
    mutation_insert(4, 1, r_map); // insert 1 site at index 2
    print_map(r_map);
    
    //mutation_delete(6, 2, r_map); // delete 1 site at index 2
    //print_map(r_map);
    
    std::cout << "indexes:\n";
 for (int i = 0; i < 8; ++i) { // i < genome.size()
        std::cout << get_ind(i, r_map) << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
 

/*
#define CATCH_CONFIG_MAIN

TEST_CASE("EmptyRange")
{
    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
    std::map<int, std::pair<int, bool>> r_map;
    
    mutation_delete(3, 1, r_map);
    mutation_insert(4, 1, r_map);
    mutation_delete(6, 2, r_map);
    
    std::vector<int> inds{0, 1, 2, 4, -1, 7, 8, 9};
    for (int i = 0; i < inds.size(); ++i) { // i < genome.size()
        REQUIRE(get_ind(i, r_map) == inds[i]);
    }
}
*/
