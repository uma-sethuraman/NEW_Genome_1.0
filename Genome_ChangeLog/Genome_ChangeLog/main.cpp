
#include <iostream>
#include <map>
#include <unordered_map>
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

void mutation_point(int ind, int number_of_sites, std::map<int, std::pair<int, bool>>& r_map) {
    // Options:
    // 1. change directly in parent by accessing thhrough get_ind
    // 2. do something similar as "insert"
}

void mutation_delete(int ind, int shift, std::map<int, std::pair<int, bool>>& change_log) {
    std::cout << "in mutation_delete" << std::endl;
    std::cout << "in ind " << ind << std::endl;
    std::cout << "in shift " << shift << std::endl;

    if (change_log.lower_bound(ind) == change_log.end()) {
        // no key is >= ind -> just add the key and shift
        change_log[ind] = std::make_pair(-shift, false);
    } else {
        auto next_key_it = change_log.lower_bound(ind);
        std::cout << "next_key_it->first: " << next_key_it->first << std::endl;
        
        // loop over all keys > ind
        // if change_log
        // if key < (curr_key + shift) erase it, it is deleted
        // if key > (curr_key + shift) replace key with (key - shift) && replace val with (val - shift)
        
        std::map<int, std::pair<int, bool>> temp_r_map;
        
        for (std::map<int, std::pair<int, bool>>::iterator it = change_log.begin(); it != next_key_it; ++it) {
            //if (it->second.second == false) {
                temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
            //} else {
                // TODO
            //}
            std::cout << "insert: " << it->first << " : "
            << it->second.first << ", " << it->second.second << std::endl;
        }
        
        for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != change_log.end(); ++it) {
            std::cout << "next_key_it->first: " << next_key_it->first << std::endl;
            
            if (it->first == ind) {
                if (it->second.second == true) {
                    temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
                    std::cout << "insert if: " << it->first << " : "
                    << it->second.first - shift << ", " << it->second.second << std::endl;
                } else {
                    temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first - shift, it->second.second)));
                    std::cout << "insert if: " << it->first - shift << " : "
                    << it->second.first - shift << ", " << it->second.second << std::endl;
                }
            } else if (it->first < (ind + shift)) {
                std::cout << "in else if" << std::endl;
                // There can be a situaltion when even thoug the key is within (ind + shift),
                // there is no next key to represent the indexes which are to the right from the deletion
                // In this case, we nee to keep the key and update it
                if (std::next(it) == change_log.end() &&
                    (it->first + it->second.first) > (ind + shift)) {
                    // last key in the map and there are still indexes to the right of the deletion
                    int num_deleted = it->first - ind; // how many deleted sites between ind and key
                    temp_r_map.insert(std::make_pair(it->first - num_deleted, std::make_pair(it->second.first - shift, it->second.second)));
                }
                if (it->second.second == true) {
                    // merge the insert intervals
                    // replace key with ind, because ind < key && key is within (ind + shift)
                    // and interval that starts at ind continues at key
                    temp_r_map.insert(std::make_pair(ind, std::make_pair(0/*it->second.first*/, true/*it->second.second*/)));
                } else {
                    // erase, so skip - don't add to new map
                }
                //std::cout << "else if - nothing to insert" << std::endl;
            } else {
                temp_r_map.insert(std::make_pair(it->first - shift, std::make_pair(it->second.first - shift, it->second.second)));
                std::cout << "insert else: " << it->first - shift << " : "
                << it->second.first - shift << ", " << it->second.second << std::endl;
            }
        }
        
        change_log = temp_r_map;
    }
}

void mutation_insert(int ind, const std::vector<int>& ins_vals, std::map<int, std::pair<int, bool>>& r_map, std::unordered_map<int, std::vector<int>>& insertions) {
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
    
    int shift = ins_vals.size();
    
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
        // Merge keys in the map if insertions overlap - Don;t need this here, read below
        auto inserted = r_map.insert(std::make_pair(ind, std::make_pair(0, true))); // emplace_hint instead of insert, as it is always inserted at the end?
//        if (inserted.first == r_map.begin()) {
            // first inserted element - nothing else to check
        // Add inserted values to the unordered_map
        insertions.insert(std::make_pair(ind, ins_vals));
        // In this case (insertion index is largest key in the map), there will never be a situation when the previous key is also insersion, because insertion is always followed bu the key shifted by the size of insertion.
//        } else {
//            // Check if previous key is insertion
//            auto prev_it = std::prev(inserted.first);
//            if (prev_it->second.second == true) { // this will never happen
//                // insertion
//                if ((inserted.first->first - prev_it->first) > shift) {
//                    // don't change previous - they don't overlap
//                    insertions.insert(std::make_pair(ind, ins_vals));
//                } else {
//                    // overlap with previous insertion (TODO replace copying)
//                    std::vector<int> temp_vec = insertions.at(prev_it->first);
//                    auto temp_shift = inserted.first->first - prev_it->first;
//                    temp_vec.insert(temp_vec.begin() + temp_shift, ins_vals.begin(), ins_vals.end());
//                }
//            }
//        }
        
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
        
        // Insert all keys < ind into new map
        for (std::map<int, std::pair<int, bool>>::iterator it = r_map.begin(); it != next_key_it; ++it) {
            //std::cout << "inserting key: " << it->first << ", value: " << it->second.first << std::endl;
            temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        
        // Insert all keys >= ind into new map
        for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != r_map.end(); ++it) {
            //std::cout << "shift: " << shift << std::endl;
            //std::cout << "inserting key: " << it->first + shift << ", value: " << it->second.first + shift << std::endl;
            if (it->second.second == true) {
                temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first, it->second.second)));
            } else {
                temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first + shift, it->second.second)));
            }
        }
            
        // the following can be optimized, as it repeats the code in "if"
        auto inserted = temp_r_map.insert(std::make_pair(ind, std::make_pair(0, true)));
        
        if (inserted.first == temp_r_map.begin()) {
            // first inserted element - nothing else to check
            // Add inserted values to the unordered_map
            insertions.insert(std::make_pair(ind, ins_vals));
            // In this case (insertion index is largest key in the map), there will never be a situation when the previous key is also insersion, because insertion is always followed bu the key shifted by the size of insertion.
        } else {
            // Check if previous key is insertion
            auto prev_it = std::prev(inserted.first);
            if (prev_it->second.second == true) {
                // insertion
                if ((inserted.first->first - prev_it->first) > shift) {
                    // don't change previous - they don't overlap
                    // Add inserted values to the unordered_map
                    insertions.insert(std::make_pair(ind, ins_vals));
                } else {
                    // overlap with previous insertion (TODO replace copying)
                    std::vector<int> temp_vec = insertions.at(prev_it->first);
                    auto temp_shift = inserted.first->first - prev_it->first;
                    temp_vec.insert(temp_vec.begin() + temp_shift, ins_vals.begin(), ins_vals.end());
                    temp_r_map.erase(ind);
                }
            }
        }
            
        
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
        else { // deletion
            return ind_curr - it_curr->second.first;
        }
    }
}

std::vector<int> offspring_recon(const std::vector<int>& genome_orig,
                                const std::map<int, std::pair<int, bool>>& change_log_map) {
        
    if (change_log_map.empty()) return genome_orig; // no mutations
    
    
    // Calculte size of the offspring genome .rbegin() will not return .rend(), because log is not empty
    int offgen_size = genome_orig.size() + change_log_map.rbegin()->second.first;
    std::vector<int> genome_offspring(offgen_size, -1);
    
    for (auto it_curr = change_log_map.begin(); it_curr != change_log_map.end(); ++it_curr) {
        auto it_next = std::next(it_curr);
        //if (it_next == change_log_map.end()) break; // TODO
        
        if (it_curr->second.second == true) {
            // keep -1
            // will need loop to place the values from map, which holds insertions
        } else {
            if (it_next == change_log_map.end()) {
                // This loop fills out the last part of the vector
                for (int ind = it_curr->first; ind < offgen_size; ++ind) {
                    int ind_orig = ind - it_curr->second.first;
                    genome_offspring[ind] = genome_orig[ind_orig];
                }
                
            } else {
                for (int ind = it_curr->first; ind < it_next->first; ++ind) {
                    int ind_orig = ind - it_curr->second.first;
                    genome_offspring[ind] = genome_orig[ind_orig];
                }
            }
        }
        
    }
    
    
    return genome_offspring;
}

#ifndef DEBUG
int main()
{
//    std::vector<int> genome{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
//
//    std::map<int, std::pair<int, bool>> change_log {{0, {0, false}}};
//
//    std::unordered_map<int, std::vector<int>> insertions;
//
//
//    std::cout << "mutation_delete(3, 1, change_log);\n";
//    mutation_delete(3, 1, change_log);
//    print_map(change_log);
//
//    std::cout << "mutation_insert(4, 1, change_log);\n";
//    mutation_insert(4, 1, change_log, insertions);
//    print_map(change_log);
//
//    std::cout << "mutation_delete(6, 2, change_log);\n";
//    mutation_delete(6, 2, change_log);
//    print_map(change_log);
//
//    std::cout << "mutation_insert(5, 3, change_log);\n";
//    mutation_insert(5, 3, change_log, insertions);
//    print_map(change_log);
//
//    std::cout << "mutation_delete(6, 3, change_log);\n";
//    mutation_delete(6, 3, change_log);
//    print_map(change_log);
//
//    std::cout << "mutation_delete(6, 2, change_log);\n";
//    mutation_delete(6, 2, change_log);
//    print_map(change_log);
//
//    std::cout << "mutation_insert(0, 3, change_log);\n";
//    mutation_insert(0, 3, change_log, insertions);
//    print_map(change_log);
    
    std::vector<int> genome_orig{21, 35, 43, 84, 12};
    std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
    std::unordered_map<int, std::vector<int>> insertions;
    
    std::cout << "mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions)\n";
    mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
    print_map(change_log);
    
    std::cout << "mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions)\n";
    mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
    print_map(change_log);
    
    std::cout << "mutation_delete(1, 3, change_log)\n";
    mutation_delete(1, 3, change_log);
    print_map(change_log);
    
    //std::cout << "mutation_delete(3, 4, change_log);\n";
    //mutation_delete(3, 4, change_log);
    //print_map(change_log);
    
    std::cout << "mutation_delete(1, 3, change_log)\n";
    mutation_delete(1, 3, change_log);
    print_map(change_log);
    
    size_t new_size = genome_orig.size() + change_log.rbegin()->second.first;
    std::cout << "size of new genome: " << new_size << std::endl;
    std::cout << "indexes:\n";
    for (int i = 0; i < new_size; ++i) {
        std::cout << get_ind(i, change_log) << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
#endif

