
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
//#include <algorithm>
//#include <memory>

#include "tests.h"


void print_map(const std::map<int, std::pair<int, bool>>& r_map) {
    std::cout << "Map:" << std::endl;
    for (auto it = r_map.begin(); it != r_map.end(); ++it) {
        std::cout << it->first << " : " <<
        it->second.first << " : " << it->second.second << std::endl;
    }
}

void print_u_map(const std::unordered_map<int, std::vector<int>>& u_r_map) {
    std::cout << "Map unordered:" << std::endl;
    for (auto it = u_r_map.begin(); it != u_r_map.end(); ++it) {
        std::cout << it->first << " : ";
        for (auto v : it->second) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}

void mutation_point(int ind, int number_of_sites, std::map<int, std::pair<int, bool>>& r_map) {
    // Options:
    // 1. change directly in parent by accessing thhrough get_ind
    // 2. do something similar as "insert"
}

void mutation_delete(int ind, int shift, std::map<int, std::pair<int, bool>>& change_log, std::unordered_map<int, std::vector<int>>& segments_log) {
    
//    std::cout << "in mutation_delete" << std::endl;
//    std::cout << "in ind " << ind << std::endl;
//    std::cout << "in shift " << shift << std::endl;

    if (change_log.lower_bound(ind) == change_log.end()) {
        // no key is >= ind -> just add the key and shift
        change_log[ind] = std::make_pair(-shift, false);
    } else {
        auto next_key_it = change_log.lower_bound(ind);
//        std::cout << "next_key_it->first: " << next_key_it->first << std::endl;

        // loop over all keys > ind
        // if change_log
        // if key < (curr_key + shift) erase it, it is deleted
        // if key > (curr_key + shift) replace key with (key - shift) && replace val with (val - shift)

        std::map<int, std::pair<int, bool>> temp_r_map;

        for (auto it = change_log.begin(); it != next_key_it; ++it) {
            temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));

//            std::cout << "insert: " << it->first << " : "
//            << it->second.first << ", " << it->second.second << std::endl;
        }
        // !
        if (next_key_it != change_log.begin() && (std::prev(next_key_it))->second.second == true) {
            // need to remove couple of values at the end of last insertion (e.g.: Delete 4 sites at index 3)
            // update segments_log
            std::vector<int> segm_to_update;
            segm_to_update = segments_log.find((std::prev(next_key_it))->first)->second;
            //auto it_from_erase = segm_to_update.begin() + (next_key_it->first - ind);
            auto it_from_erase = segm_to_update.end() - (next_key_it->first - ind); // !
//            std::cout << "erasing from index: " << -(next_key_it->first - ind) << " till end" << std::endl;
            segm_to_update.erase(it_from_erase, segm_to_update.end());
            segments_log.find((std::prev(next_key_it))->first)->second = segm_to_update;
        }

        for (auto it = next_key_it; it != change_log.end(); ++it) {
//            std::cout << "it->first: " << it->first << std::endl;

            if (it->first < (ind + shift)) {
//                std::cout << "in if" << std::endl;
                if (it->second.second == false) {
//                    std::cout << "don't add " << it->first << " to the map" << std::endl;
                    // erase, so skip - don't add to new map ??
                    if (std::next(it) == change_log.end()) {
                        // special case when the key is the last in the map
                        int new_key = it->first - (it->first - ind);
                        temp_r_map.insert({{new_key, {it->second.first - shift, it->second.second}}});
//                        std::cout << "insert: " << new_key << " : " << it->second.first - shift << ", " << it->second.second << std::endl;
                    }
                } else { // it->second.second == true
//                    std::cout << "update " << it->first << " in both maps" << std::endl;
                    if (it->first + (segments_log.find(it->first)->second).size() <= (ind + shift)) {
//                        std::cout << "erasing " <<  it->first << " in both maps" << std::endl;
                        int to_erase = it->first;
                        //change_log.erase(to_erase); // just don't add it to the temp_r_map
                        segments_log.erase(to_erase);
                    } else {
//                        std::cout << "merge " <<  it->first << " in both maps" << std::endl;
                        // only erase the first (ind + shift) - key) elements
                        // update change_log
                        temp_r_map.insert({{ind /* OR it->first - shift OR min(ind,it->first) */, {0, true}}});
//                        std::cout << "insert: " << ind << " : " << 0 << ", " << true << std::endl;
                        
                        // update segments_log
                        std::vector<int> segm_to_merge;
                        segm_to_merge = segments_log.find(it->first)->second;
                        auto it_till_erase = segm_to_merge.begin() + (ind + shift - it->first);
                        segm_to_merge.erase(segm_to_merge.begin(), it_till_erase);
                        segments_log.find(it->first)->second = segm_to_merge;
                        
                        auto key_to_shift = segments_log.extract(it->first);
                        key_to_shift.key() = ind /* OR it->first - shift OR min(ind,it->first) */;
                        segments_log.insert(move(key_to_shift));
                    }
                        
                }
                
            } else { // it->first >= (ind + shift): (key - shift, val - shift)
//                std::cout << "in else" << std::endl;
                // update change_log
                
//                if ((it->first == (ind + shift)) && (it->second.second == true)) {
//
//                    std::cout << "in else: if" << std::endl;
//                    std::cout << "it->first = " << it->first << std::endl;
//
//                    // TODO: do i need this condition: it->first == (ind + shift)? i think i do, cause if it is >, this is not needed
//                    // merge with prev
//                    std::vector<int> segm_merged = segments_log.find((std::prev(it))->first)->second;
//                    std::vector<int> segm_to_merge = segments_log.find(it->first)->second;
//                    segm_merged.insert(segm_merged.end(), segm_to_merge.begin(), segm_to_merge.end());
//                    segments_log.find((std::prev(it))->first)->second = segm_merged;
//
//                    int to_erase = it->first;
//                    //change_log.erase(to_erase); // just don't add it
//                    segments_log.erase(to_erase);
//
//
//
//                } else {
//                std::cout << "in else: else" << std::endl;
//                std::cout << "it->first = " << it->first << std::endl;
                
                
                int updated_val = (it->second.second == true) ? 0 : (it->second.first - shift);
                temp_r_map.insert(std::make_pair(it->first - shift, std::make_pair(updated_val, it->second.second)));
//                std::cout << "insert: " << (it->first - shift) << " : " << updated_val << ", " << it->second.second << std::endl;
                
                // update segments_log
                if (it->second.second == true) {
                    auto key_to_shift = segments_log.extract(it->first);
                    key_to_shift.key() = it->first - shift;
                    segments_log.insert(move(key_to_shift));
                }
                //                }
            }
            
        }

        change_log = temp_r_map;
        
        // Go through maps and merge insertions if they overlap
        for (auto it = change_log.begin(); it != change_log.end(); ++it) {
            if (it->second.second == true) {
                if (std::next(it) != change_log.end() && (std::next(it))->second.second == true) {
//                    std::cout << "merging two inserts" << std::endl;
                    auto next_it = std::next(it);
                    std::vector<int> segm_merged = segments_log.find(it->first)->second;
                    std::vector<int> segm_to_merge = segments_log.find(next_it->first)->second;
                    segm_merged.insert(segm_merged.end(), segm_to_merge.begin(), segm_to_merge.end());
                    segments_log.find(it->first)->second = segm_merged; // rempace in segments_log
                    
                    int to_erase = next_it->first;
                    change_log.erase(to_erase);
                    segments_log.erase(to_erase);
                }
            }
        }
        
        if (change_log.empty()) {
            change_log.insert({{0, {0, false}}});
        }

        if (change_log.size() == 1 && change_log.begin()->first == 0 && change_log.begin()->second.first < 0) {
            // this means that everything was deleted
            change_log[0] = {0, false};
        }
    }
}

//void mutation_delete(int ind, int shift, std::map<int, std::pair<int, bool>>& change_log, std::unordered_map<int, std::vector<int>>& segments_log) {
//
//    auto lb_key = change_log.lower_bound(ind); // can be end()
//
//    std::map<int, std::pair<int, bool>> change_log_temp;
//
//    for (auto it = change_log.begin(); it != lb_key; ++it) {
//        change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
//    }
//    for (auto it = lb_key; it != change_log.end(); ++it) {
//        if (it->second.second == false) {
//
//        }
//    }
//}

void mutation_insert(int ind, const std::vector<int>& segment, std::map<int, std::pair<int, bool>>& change_log, std::unordered_map<int, std::vector<int>>& segments_log) {
    
    std::cout << "==================" << std::endl;
    std::cout << "in mutation_insert" << std::endl;
    std::cout << "at ind: " << ind << ", segment: ";
    for (auto& v : segment)
        std::cout << v << " ";
    std::cout << std::endl;
    
    int shift = segment.size();
    
    auto next_key = change_log.upper_bound(ind); // can be end()
    auto lb_key = change_log.lower_bound(ind); // can be end()
    auto prev_key = std::prev(change_log.upper_bound(ind)); // always valid, as change_log.size() >= 1
    //auto found_key = change_log.find(ind);
    
    if (lb_key == change_log.end()) {
        // the last key will always be false, so if there nothing <= than ind: just add ind to both maps
        
        // update change_log
        change_log.insert({{ind, {0, true}}});
        change_log.insert({{ind + shift, {prev_key->second.first + shift, false}}});
        
        std::cout << "inserting key: " << ind << ", val: (" <<  0 << ", " << true << ")" << std::endl;
        std::cout << "inserting key: " << ind + shift << ", val: (" << prev_key->second.first + shift << ", " << false << ")" << std::endl;

                          
        // update segments_log
        segments_log.insert({{ind, segment}});
        
        return;
    }
    
    if (next_key == change_log.end()) {
        // next_key will only be end() if
        // lb_key is also end() - took care of this case before
        // lb_key is not end(): this means that ind is the last key in the map => last key in the map will always be false, so this will be taken care of in the "else" condition
    }
    
    std::cout << "next_key: " << next_key->first << ": " << next_key->second.first << ", " << next_key->second.second << std::endl;
    std::cout << "lb_key: " << lb_key->first << ": " << lb_key->second.first << ", " << lb_key->second.second << std::endl;
    std::cout << "prev_key: " << prev_key->first << ": " << prev_key->second.first << ", " << prev_key->second.second << std::endl;
    

    std::map<int, std::pair<int, bool>> change_log_temp;
    
    if (prev_key->second.second == true) {
        std::cout << "in if" << std::endl;
        // update change_log
        for (auto it = change_log.begin(); it != std::next(prev_key); ++it) {
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        for (auto it = std::next(prev_key); it != change_log.end(); ++it) {
            change_log_temp.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first + shift, it->second.second)));
        }
        
        // update segments_log
        auto it_to_merge = segments_log.find(prev_key->first); // find is O(1) here
        std::vector<int> segm_to_merge;
        if (it_to_merge != segments_log.end()) {
            segm_to_merge = it_to_merge->second;
            segm_to_merge.insert(segm_to_merge.begin() + (ind - prev_key->first), segment.begin(), segment.end());
            // TODO: what is segm_to_merge.begin() + (ind - prev_key->first) is our of range?
            it_to_merge->second = segm_to_merge; // replace with merged segment in segments_log
        } else {
            // should not happen, because prev_key->second.second == true
            std::cout << "SEGMENTS MAP IS WRONG!" << std::endl;
        }
        
    } else if (next_key != change_log.end() && next_key->second.second == true) {
        if (lb_key->first != ind) {
            std::cout << "in else if: if" << std::endl;
            // update change_log
            for (auto it = change_log.begin(); it != prev_key; ++it) {
                change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
            }
            
            std::cout << "inserting key: " << prev_key->first << ", val: (" <<  ind << " - " << prev_key->second.first << ")" << std::endl;
            change_log_temp.insert(std::make_pair(prev_key->first, std::make_pair(ind - prev_key->second.first, prev_key->second.second)));
            
            for (auto it = next_key; it != change_log.end(); ++it) { // ???
                int updated_val = (it->second.second == true) ? 0 : (it->second.first + shift);
                change_log_temp.insert(std::make_pair(it->first + shift, std::make_pair(updated_val, it->second.second)));
                
                // update segments_log
                if (it->second.second == true) {
                    auto key_to_shift = segments_log.extract(it->first);
                    key_to_shift.key() = it->first + shift;
                    segments_log.insert(move(key_to_shift));
                }
            }
            //change_log_temp.insert({{ind, {0, true}}});
            change_log_temp[ind] = {0, true}; // want to replace if exists
                                              
            // ???
//            std::cout << "inserting key: " << ind + shift << ", val: (" << next_key->first << " - " << ind << ")" << std::endl;
//            change_log_temp.insert({{ind + shift, {next_key->first - ind, false}}});
//            std::cout << "inserting key: " << ind + shift << ", val: (" << prev_key->second.first << " - " << shift << ")" << std::endl;
            change_log_temp.insert({{ind + shift, {prev_key->second.first + shift, false}}});
            // ???
            
            // update segments_log
            segments_log.insert({{ind, segment}});
        } else {
            std::cout << "in else if: else" << std::endl;
            // update change_log
            for (auto it = change_log.begin(); it != lb_key; ++it) { // is lb_key==prev_key always?
                change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
            }
            for (auto it = lb_key; it != change_log.end(); ++it) {
                int updated_val = (it->second.second == true) ? 0 : (it->second.first + shift);
                change_log_temp.insert(std::make_pair(it->first + shift, std::make_pair(updated_val, it->second.second)));
                
                // update segments_log
                if (it->second.second == true) {
                    auto key_to_shift = segments_log.extract(it->first);
                    key_to_shift.key() = it->first + shift;
                    segments_log.insert(move(key_to_shift));
                }
            }
            //change_log_temp.insert({{ind, {0, true}}});
            change_log_temp[ind] = {0, true}; // want to replace if exists
            
            // update segments_log
            segments_log.insert({{ind, segment}});
        }
        
    } else {
        std::cout << "in else" << std::endl;
        // no insertions before nor after ind
        // also takes care of "next_key == change_log.end()"
        // update change_log
        for (auto it = change_log.begin(); it != lb_key; ++it) {
//            std::cout << "should not enter here" << std::endl;
//            std::cout << "inserting key: " << it->first << ", val: (" << it->second.first << ", " << it->second.second << ")" << std::endl;
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        for (auto it = lb_key; it != change_log.end(); ++it) {
            // !!!
            int updated_val = (it->second.second == true) ? 0 : (it->second.first + shift);
            change_log_temp.insert(std::make_pair(it->first + shift, std::make_pair(updated_val, it->second.second)));
            std::cout << "8: inserting key: " << it->first + shift << ", val: (" << updated_val << ", " << it->second.second << ")" << std::endl;
            
            // update segments_log
            if (it->second.second == true) {
                auto key_to_shift = segments_log.extract(it->first);
                key_to_shift.key() = it->first + shift;
                segments_log.insert(move(key_to_shift));
            }
        }
        std::cout << "9: inserting key: " << ind << ", val: (" << 0 << ", " << true << ")" << std::endl;
        //change_log_temp.insert(std::make_pair(ind, std::make_pair(0, false))); // Jul 13: why false?
        change_log_temp.insert(std::make_pair(ind, std::make_pair(0, true)));
        
        // !!!
        if ((ind + shift) <= next_key->first) { //next_key == end() already checked
            change_log_temp[ind + shift] = {shift, false};
        }
        
        // update segments_log
        segments_log.insert(std::make_pair(ind, segment));
    }
    
    change_log = change_log_temp; // is this constant time?
    
    // Go through maps and merge insertions if they overlap
    for (auto it = change_log.begin(); it != change_log.end(); ++it) {
        //std::cout << "In merging loop" << std::endl;
        //std::cout << "it: " << it->first << ": " << it->second.first << ", " << it->second.second << std::endl;
        if (it->second.second == true) {
            if (std::next(it) != change_log.end() && (std::next(it))->second.second == true) {
                std::cout << "merging two inserts" << std::endl;
                auto next_it = std::next(it);
                std::vector<int> segm_merged = segments_log.find(it->first)->second;
                std::vector<int> segm_to_merge = segments_log.find(next_it->first)->second;
                segm_merged.insert(segm_merged.end(), segm_to_merge.begin(), segm_to_merge.end());
                segments_log.find(it->first)->second = segm_merged; // rempace in segments_log
                
                int to_erase = next_it->first;
                change_log.erase(to_erase);
                segments_log.erase(to_erase);
            }
        }
    }
    
    /*
     if prev_key(=prev(upper_bound(ind)), so prev_key could be ==ind) true (overlap: merge segment into prev_key segment):
     (green arrow on drawing)
     for keys < upper_key: keep the same; keys>=upper_key: move by shift both key and val;
     upper_key:
     1.next(ind) if prev==ind: need to keep key==ind in the map
     2.ind if prev!=ind: don’t need to keep key!=ind in the map
     next(prev_key) satisfies both, right?
     e.g. if prev==ind==4: next(prev) will be 6 => good, I want to keep 4 the same in the map
     if prev==4, ind==5; prev!=ind: still next(prev): will be 6 => good, I want to keep 4 the same in the map, but not 5

     */
    
}

//void mutation_insert(int ind, const std::vector<int>& ins_vals, std::map<int, std::pair<int, bool>>& r_map, std::unordered_map<int, std::vector<int>>& insertions) {
//    std::cout << "in mutation_insert" << std::endl;
//
//    int shift = ins_vals.size();
//
//    auto next_key_it = r_map.lower_bound(ind);
//
//    if (next_key_it == r_map.end()) {
//        // Inserting at the end of the map, no other keys are affected
//
//        auto inserted = r_map.insert(std::make_pair(ind, std::make_pair(0, true)));
//        std::cout << "inserting: " << ind << ", ";
//        for (auto v : ins_vals)
//            std::cout << v << " ";
//        std::cout << std::endl;
//        insertions.insert(std::make_pair(ind, ins_vals));
//
//        // Move the ones after it
//        if (inserted.first == r_map.begin()) // map was empty just before the previous line
//            r_map.insert(std::make_pair(ind + shift, std::make_pair(shift, false)));
//        else
//            r_map.insert(std::make_pair(ind + shift, std::make_pair((--inserted.first)->second.first + shift, false)));
//
//    } else {
//        // ind is inserted between existing keys - need to update the keys >= ind
//
//        std::map<int, std::pair<int, bool>> temp_r_map;
//
//        // Insert all keys < ind into new map
//        for (std::map<int, std::pair<int, bool>>::iterator it = r_map.begin(); it != next_key_it; ++it) {
//            std::cout << "inserting key: " << it->first << ", value: " << it->second.first << std::endl;
//            temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
//        }
//
//        // Insert all keys >= ind into new map
//        // AND update the unordered_map insertions
//        for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != r_map.end(); ++it) {
//            //std::cout << "shift: " << shift << std::endl;
//            std::cout << "inserting key: " << it->first + shift << ", value: " << it->second.first + shift << std::endl;
//            if (it->second.second == true) {
//                // also need to update u_map
//                temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first, it->second.second)));
//
//                // also need to update u_map
//                auto nh = insertions.extract(it->first);
//                nh.key() = it->first + shift;
//                insertions.insert(move(nh));
//            } else {
//                temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first + shift, it->second.second)));
//            }
//        }
//
//        // the following can be optimized, as it repeats the code in "if"
//        auto inserted = temp_r_map.insert(std::make_pair(ind, std::make_pair(0, true)));
//
//        if (inserted.first == temp_r_map.begin()) {
//            // first inserted element - nothing else to check
//            // Add inserted values to the unordered_map
//            std::cout << "inserting: " << ind << ", ";
//            for (auto v : ins_vals)
//                std::cout << v << " ";
//            std::cout << std::endl;
//            insertions.insert(std::make_pair(ind, ins_vals));
//            // In this case (insertion index is largest key in the map), there will never be a situation when the previous key is also insersion, because insertion is always followed bu the key shifted by the size of insertion.
//        } else {
//            // Check if previous key is insertion
//            auto prev_it = std::prev(inserted.first);
//            if (prev_it->second.second == true) {
//                // insertion
//                if ((inserted.first->first - prev_it->first) > shift) {
//                    // don't change previous - they don't overlap
//                    // Add inserted values to the unordered_map
//                    insertions.insert(std::make_pair(ind, ins_vals));
//                } else {
//                    // overlap with previous insertion (TODO replace copying)
//                    std::vector<int> temp_vec = insertions.at(prev_it->first);
//                    auto temp_shift = inserted.first->first - prev_it->first;
//                    temp_vec.insert(temp_vec.begin() + temp_shift, ins_vals.begin(), ins_vals.end());
//                    temp_r_map.erase(ind);
//                }
//            }
//        }
//
//
//        r_map = temp_r_map;
//
//    }
//}



//void mutation_insert(int ind, const std::vector<int>& ins_vals, std::map<int, std::pair<int, bool>>& r_map, std::unordered_map<int, std::vector<int>>& insertions) {
//    std::cout << "in mutation_insert" << std::endl;
//
//    /*
//     Insert:
//     - add index, # of sites to shift and true
//     Cases:
//     - ind > largest key in map => just insert ind and val = 0
//     - ind <= largest key in map =>
//     - add ind with val = 0
//     - every key >= ind has to be updated
//
//     */
//
//    int shift = ins_vals.size();
//
//    //auto next_key_it = r_map.upper_bound(ind); // == r_map.end() if there is no value > ind in the map
//    auto next_key_it = r_map.lower_bound(ind); // == r_map.end() if there is no value >= ind in the map
//                                               // I think lower_bound should be used, because the situation when ind is already in the map
//                                               //  requires separate processing
//
//    //std::cout << "ind: " << ind << std::endl;
//    //if (next_key_it != r_map.end())
//    //    std::cout << "next_key_it->first: " << next_key_it->first << std::endl;
//    //else
//    //    std::cout << "next_key_it == r_map.end()" << std::endl;
//
//    if (next_key_it == r_map.end()) { // ind > largest key in map
//                                      // there is no key in the map that is either == or > than ind
//                                      // => add the ind and val = 0; then add ind + shift and val = shift
//                                      // this meant that for ind ... ind + shift the val = 0 (inserted sites)
//                                      // starting from ind_shift the val =  + shift (take previous shift and add new one)
//
//        // Insert current ind
//        // Merge keys in the map if insertions overlap - Don;t need this here, read below
//        auto inserted = r_map.insert(std::make_pair(ind, std::make_pair(0, true))); // emplace_hint instead of insert, as it is always inserted at the end?
//                                                                                    //        if (inserted.first == r_map.begin()) {
//                                                                                    // first inserted element - nothing else to check
//                                                                                    // Add inserted values to the unordered_map
//        std::cout << "inserting: " << ind << ", ";
//        for (auto v : ins_vals)
//            std::cout << v << " ";
//        std::cout << std::endl;
//        insertions.insert(std::make_pair(ind, ins_vals));
//        // In this case (insertion index is largest key in the map), there will never be a situation when the previous key is also insersion, because insertion is always followed bu the key shifted by the size of insertion.
//        //        } else {
//        //            // Check if previous key is insertion
//        //            auto prev_it = std::prev(inserted.first);
//        //            if (prev_it->second.second == true) { // this will never happen
//        //                // insertion
//        //                if ((inserted.first->first - prev_it->first) > shift) {
//        //                    // don't change previous - they don't overlap
//        //                    insertions.insert(std::make_pair(ind, ins_vals));
//        //                } else {
//        //                    // overlap with previous insertion (TODO replace copying)
//        //                    std::vector<int> temp_vec = insertions.at(prev_it->first);
//        //                    auto temp_shift = inserted.first->first - prev_it->first;
//        //                    temp_vec.insert(temp_vec.begin() + temp_shift, ins_vals.begin(), ins_vals.end());
//        //                }
//        //            }
//        //        }
//
//        // Move the ones after it
//        if (inserted.first == r_map.begin()) // map was empty just before the previous line
//            r_map.insert(std::make_pair(ind + shift, std::make_pair(shift, false)));
//        else
//            r_map.insert(std::make_pair(ind + shift, std::make_pair((--inserted.first)->second.first + shift, false)));
//
//    } else { // ind <= largest key in map
//             // whether ind == next_key_it or < next_key_it
//             // go through keys >= next_key_it and add shift
//             // important to start with next_key_it, because if next_key_it == ind
//             // we will lose the info at next_key_it when inserting (ind, 0)
//             //for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != r_map.end(); ++it) {
//             //    it->first += shift;
//             //    it->second += shift;
//             //}
//
//        std::map<int, std::pair<int, bool>> temp_r_map;
//
//        // Insert all keys < ind into new map
//        for (std::map<int, std::pair<int, bool>>::iterator it = r_map.begin(); it != next_key_it; ++it) {
//            std::cout << "inserting key: " << it->first << ", value: " << it->second.first << std::endl;
//            temp_r_map.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
//        }
//
//        // Insert all keys >= ind into new map
//        for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != r_map.end(); ++it) {
//            //std::cout << "shift: " << shift << std::endl;
//            std::cout << "inserting key: " << it->first + shift << ", value: " << it->second.first + shift << std::endl;
//            if (it->second.second == true) {
//                temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first, it->second.second)));
//            } else {
//                temp_r_map.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first + shift, it->second.second)));
//            }
//        }
//
//        // the following can be optimized, as it repeats the code in "if"
//        auto inserted = temp_r_map.insert(std::make_pair(ind, std::make_pair(0, true)));
//
//        if (inserted.first == temp_r_map.begin()) {
//            // first inserted element - nothing else to check
//            // Add inserted values to the unordered_map
//            std::cout << "inserting: " << ind << ", ";
//            for (auto v : ins_vals)
//                std::cout << v << " ";
//            std::cout << std::endl;
//            insertions.insert(std::make_pair(ind, ins_vals));
//            // In this case (insertion index is largest key in the map), there will never be a situation when the previous key is also insersion, because insertion is always followed bu the key shifted by the size of insertion.
//        } else {
//            // Check if previous key is insertion
//            auto prev_it = std::prev(inserted.first);
//            if (prev_it->second.second == true) {
//                // insertion
//                if ((inserted.first->first - prev_it->first) > shift) {
//                    // don't change previous - they don't overlap
//                    // Add inserted values to the unordered_map
//                    insertions.insert(std::make_pair(ind, ins_vals));
//                } else {
//                    // overlap with previous insertion (TODO replace copying)
//                    std::vector<int> temp_vec = insertions.at(prev_it->first);
//                    auto temp_shift = inserted.first->first - prev_it->first;
//                    temp_vec.insert(temp_vec.begin() + temp_shift, ins_vals.begin(), ins_vals.end());
//                    temp_r_map.erase(ind);
//                }
//            }
//        }
//
//
//        r_map = temp_r_map;
//
//    }
//
//    //std::cout << "prev_it: " << prev_it->first << std::endl;
//    //r_map[ind] = std::make_pair(0, true);
//    //r_map[ind + shift] = std::make_pair(prev_it->second.first - shift, false);
//}

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
                                const std::map<int, std::pair<int, bool>>& change_log_map, const std::unordered_map<int, std::vector<int>>& insertions) {
        
    if (change_log_map.size() == 1) {
        if (change_log_map.begin()->first == 0 && change_log_map.begin()->second.first == 0 && change_log_map.begin()->second.second == false)
            return genome_orig; // no mutations
    }
    
    
    // Calculte size of the offspring genome .rbegin() will not return .rend(), because log is not empty
    int offgen_size = genome_orig.size() + change_log_map.rbegin()->second.first;
    std::cout << "offgen_size: " << offgen_size << std::endl;
    std::vector<int> genome_offspring(offgen_size, -1);
    
    for (auto it_curr = change_log_map.begin(); it_curr != change_log_map.end(); ++it_curr) {
        auto it_next = std::next(it_curr);
        //if (it_next == change_log_map.end()) break; // TODO
        
        std::cout << "it_curr->first: " << it_curr->first << std::endl;
        std::cout << "it_next->first: " << it_next->first << std::endl;

        if (it_curr->second.second == true) {
            std::cout << "it_curr->second.second == true" << std::endl;
            std::vector<int> vals_to_insert = insertions.at(it_curr->first);
            if (it_next == change_log_map.end()) { // will this ever happen?
                                               // This loop fills out the last part of the vector
                int ind_to_insert = 0;
                for (int ind = it_curr->first; ind < offgen_size; ++ind) {
                    genome_offspring[ind] = vals_to_insert[ind_to_insert];
                    ind_to_insert++;
                }
            } else {
                int ind_to_insert = 0;
                for (int ind = it_curr->first; ind < it_next->first; ++ind) {
                    genome_offspring[ind] = vals_to_insert[ind_to_insert];
                    ind_to_insert++;
                }
            }
        } else {
            std::cout << "it_curr->second.second != true" << std::endl;
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

//#ifndef DEBUG
int main(int argc, char* argv[])
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
    
    
    
    
    
// ============================================
//    std::vector<int> genome_orig{21, 35, 43, 84, 12};
//    std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
//    std::unordered_map<int, std::vector<int>> insertions;
//
//    std::cout << "mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions)\n";
//    mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
//    print_map(change_log);
//
//    std::cout << "mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions)\n";
//    mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
//
//    std::vector<int> offspr = offspring_recon(genome_orig, change_log, insertions);
//    for (auto v : offspr) {
//        std::cout << v << " ";
//    }
//    std::cout << std::endl;
//
//    std::cout << "===============================" << std::endl;
//    std::vector<int> genome_orig2{};
//    std::map<int, std::pair<int, bool>> change_log2{{0, {0, false}}};
//    std::unordered_map<int, std::vector<int>> insertions2;
//
//    std::cout << "mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions)\n";
//    mutation_insert(0, std::vector<int>{1, 2, 3}, change_log2, insertions2);
//    print_map(change_log2);
//
//    std::cout << "mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions)\n";
//    mutation_insert(0, std::vector<int>{4, 5, 6}, change_log2, insertions2);
//    print_map(change_log2);
//    print_u_map(insertions2);
//
//    std::vector<int> offspr2 = offspring_recon(genome_orig2, change_log2, insertions2);
//    for (auto v : offspr2) {
//        std::cout << v << " ";
//    }
//    std::cout << std::endl;
    
    
    
// ==================================================
    //std::cout << "mutation_delete(1, 3, change_log)\n";
    //mutation_delete(1, 3, change_log);
    //print_map(change_log);
    
    //std::cout << "mutation_delete(3, 4, change_log);\n";
    //mutation_delete(3, 4, change_log);
    //print_map(change_log);
    
    //std::cout << "mutation_delete(1, 3, change_log)\n";
    //mutation_delete(1, 3, change_log);
    //print_map(change_log);
    
    //size_t new_size = genome_orig.size() + change_log.rbegin()->second.first;
    //std::cout << "size of new genome: " << new_size << std::endl;
    //std::cout << "indexes:\n";
    //for (int i = 0; i < new_size; ++i) {
    //    std::cout << get_ind(i, change_log) << " ";
    //}
    //std::cout << std::endl;
    
    
    
// ===================================================
//    std::vector<int> genome_orig{1, 2, 3};
//    std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
//    std::unordered_map<int, std::vector<int>> insertions;
//
//    std::cout << "mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions)\n";
//    mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
//    print_map(change_log);
//
//    std::cout << "mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions)\n";
//    mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
    
    
    
    
    
    
    
// =====================================
//    std::vector<int> genome_orig{6, 7, 1, 2, 4, 5, 3};
//    std::map<int, std::pair<int, bool>> change_log{
//        {0, {0, true}},
//        {2, {2, false}},
//        {4, {0, true}},
//        {6, {4, false}}
//    };
//    std::unordered_map<int, std::vector<int>> insertions{
//        {0, {6, 7}},
//        {4, {4, 5}}
//    };
//
//    std::cout << "mutation_insert(5, std::vector<int>{8, 9}, change_log, insertions)\n";
//    mutation_insert(5, std::vector<int>{8, 9}, change_log, insertions);
//
//    print_map(change_log);
//    print_u_map(insertions);
//
//
//    std::vector<int> genome_orig2{6, 7, 1, 2, 4, 5, 3};
//    std::map<int, std::pair<int, bool>> change_log2{
//        {0, {0, true}},
//        {2, {2, false}},
//        {4, {0, true}},
//        {6, {4, false}}
//    };
//    std::unordered_map<int, std::vector<int>> insertions2{
//        {0, {6, 7}},
//        {4, {4, 5}}
//    };
//
//    std::cout << "mutation_insert(5, std::vector<int>{8, 9}, change_log2, insertions2)\n";
//    mutation_insert(4, std::vector<int>{8, 9}, change_log2, insertions2);
//
//    print_map(change_log2);
//    print_u_map(insertions2);
    
    
    
//    //========================================
//    std::vector<int> genome_orig3{1, 2, 3};
//    std::map<int, std::pair<int, bool>> change_log3{
//        {0, {0, false}}
//    };
//    std::unordered_map<int, std::vector<int>> insertions3{};
//
//    std::cout << "Before insertions: " << std::endl;
//    print_map(change_log3);
//    print_u_map(insertions3);
//
//    std::cout << "mutation_insert(2, std::vector<int>{4, 5}, change_log3, insertions3)\n";
//    mutation_insert(2, std::vector<int>{4, 5}, change_log3, insertions3);
//    print_map(change_log3);
//    print_u_map(insertions3);
//
//    std::cout << "then mutation_insert(0, std::vector<int>{6, 7}, change_log3, insertions3)\n";
//    mutation_insert(0, std::vector<int>{6, 7}, change_log3, insertions3);
//    print_map(change_log3);
//    print_u_map(insertions3);
//
////    // 1.
////    std::cout << "then mutation_insert(5, std::vector<int>{8, 9}, change_log3, insertions3)\n";
////    mutation_insert(5, std::vector<int>{8, 9}, change_log3, insertions3);
////    print_map(change_log3);
////    print_u_map(insertions3);
//
////    // 2.
////    std::cout << "then mutation_insert(4, std::vector<int>{8, 9}, change_log3, insertions3)\n";
////    mutation_insert(4, std::vector<int>{8, 9}, change_log3, insertions3);
////    print_map(change_log3);
////    print_u_map(insertions3);
//
//    // 3.
//    std::cout << "then mutation_insert(3, std::vector<int>{8, 9}, change_log3, insertions3)\n";
//    mutation_insert(3, std::vector<int>{8, 9}, change_log3, insertions3);
//    print_map(change_log3);
//    print_u_map(insertions3);
//
//    std::cout << "then mutation_delete(5, 3, change_log3)\n";
//    mutation_delete(5, 3, change_log3, insertions3);
//    print_map(change_log3);
//    print_u_map(insertions3);
//
//    std::cout << "then mutation_delete(1, 2, change_log3)\n";
//    mutation_delete(1, 2, change_log3, insertions3);
//    print_map(change_log3);
//    print_u_map(insertions3);
//
//    std::cout << "then mutation_delete(0, 10, change_log3)\n";
//    mutation_delete(0, 10, change_log3, insertions3);
//    print_map(change_log3);
//    print_u_map(insertions3);
    
    
    
    
//    //===========================================================
//    std::vector<int> genome_orig{21, 35, 43, 84, 12};
//    std::map<int, std::pair<int, bool>> change_log{{0, {0, false}}};
//    std::unordered_map<int, std::vector<int>> insertions;
//
//    std::cout << "mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions)\n";
//    mutation_insert(2, std::vector<int>{1, 2, 3, 4}, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
//
//    std::cout << "then mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions)\n";
//    mutation_insert(3, std::vector<int>{5, 6}, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
//
//    std::cout << "then  mutation_delete(1, 3, change_log, insertions)\n";
//    mutation_delete(1, 3, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
//
////    std::cout << "then  mutation_delete(1, 3, change_log, insertions)\n";
////    mutation_delete(1, 3, change_log, insertions);
////    print_map(change_log);
////    print_u_map(insertions);
//
//    std::cout << "then  mutation_delete(3, 4, change_log, insertions)\n";
//    mutation_delete(3, 4, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);

    
    
    
    
    
    
//    //========================================
//    std::vector<int> genome_orig{};
//    std::map<int, std::pair<int, bool>> change_log{
//        {0, {0, false}}
//    };
//    std::unordered_map<int, std::vector<int>> insertions{};
//
//    std::cout << "Before insertions: " << std::endl;
//    print_map(change_log);
//    print_u_map(insertions);
//
//    std::cout << "mutation_insert(0, std::vector<int>{1, 2, 3}, change_log, insertions)\n";
//    mutation_insert(0, std::vector<int>{1, 2, 3}, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
//
//    std::cout << "mutation_insert(0, std::vector<int>{1, 2, 3}, change_log, insertions)\n";
//    mutation_insert(0, std::vector<int>{1, 2, 3}, change_log, insertions);
//    print_map(change_log);
//    print_u_map(insertions);
    
    
    
    
    //========================================
    std::vector<int> genome_orig{21, 35, 43, 84, 12, 15, 19, 28, 39, 45, 51};
    std::map<int, std::pair<int, bool>> change_log{
        {0, {0, false}}
    };
    std::unordered_map<int, std::vector<int>> insertions{};
    
    std::cout << "Before insertions: " << std::endl;
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_delete(3, 1)\n";
    mutation_delete(3, 1, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_insert(4, std::vector<int>{1}, change_log, insertions)\n";
    mutation_insert(4, std::vector<int>{1}, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_delete(6, 2)\n";
    mutation_delete(6, 2, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_insert(5, std::vector<int>{2, 3, 4}, change_log, insertions)\n";
    mutation_insert(5, std::vector<int>{2, 3, 4}, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_delete(6, 3)\n";
    mutation_delete(6, 3, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_delete(6, 2)\n";
    mutation_delete(6, 2, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::cout << "mutation_insert(0, std::vector<int>{5, 6, 7}, change_log, insertions)\n";
    mutation_insert(0, std::vector<int>{5, 6, 7}, change_log, insertions);
    print_map(change_log);
    print_u_map(insertions);
    
    std::vector<int> off_genome = offspring_recon(genome_orig, change_log, insertions);
    for (int v : off_genome) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    
//    int result = Catch::Session().run( argc, argv );
//    return result;
    
}
//#endif

