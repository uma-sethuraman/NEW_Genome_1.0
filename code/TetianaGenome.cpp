#include "TetianaGenome.h"
#include <iostream>

TetianaGenome::TetianaGenome(size_t _size): AbstractGenome(_size),sites(_size){
    sites.resize(_size);
}

size_t TetianaGenome::size() {
    int g_size = sites.size() + change_log.rbegin()->second.first;
    return g_size;
    
}

std::byte* TetianaGenome::data(size_t index, size_t byteSize) {

    bool mutations = false;
    
    if (change_log.size() == 1 && (change_log.begin()->first == 0 && change_log.begin()->second.first == 0 && change_log.begin()->second.second == false)) {
        mutations = false;
    } else {
        mutations = true;
    }
    
    if (mutations == false) {
        return static_cast<std::byte*>(&sites[index]); // cast is for demonstration

    } else {
        std::vector<std::byte> offspring_sites = offspring_recon();
        return static_cast<std::byte*>(&offspring_sites[index]); // cast is for demonstration
    }
}
//std::byte* TetianaGenome::data(size_t index, size_t byteSize) {
//    return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
//}

void TetianaGenome::resize(size_t new_size) {
    size_ = new_size;
};

void TetianaGenome::remove(size_t index, size_t segmentSize) {
    // change_log
    // segments_log
    
    if (empty == true) return;
    
    if (change_log.lower_bound(index) == change_log.end()) {
        // no key is >= ind -> just add the key and shift
        change_log[index] = std::make_pair(-segmentSize, false);
    } else {
        auto next_key_it = change_log.lower_bound(index);
        //std::cout << "next_key_it->first: " << next_key_it->first << std::endl;

        std::map<int, std::pair<int, bool>> change_log_temp;
        
        for (auto it = change_log.begin(); it != next_key_it; ++it) {
            // for keys < ind: keep without change
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        
        if (next_key_it != change_log.begin() && (std::prev(next_key_it))->second.second == true) {
            // update segments_log: need to remove couple of values at the end of last insertion
            std::vector<std::byte> segm_to_update;
            segm_to_update = segments_log.find((std::prev(next_key_it))->first)->second;
            //auto it_from_erase = segm_to_update.begin() + (next_key_it->first - index);
            auto it_from_erase = segm_to_update.end() - (next_key_it->first - index);
            segm_to_update.erase(it_from_erase, segm_to_update.end());
            segments_log.find((std::prev(next_key_it))->first)->second = segm_to_update;
        }
        
        for (auto it = next_key_it; it != change_log.end(); ++it) {
            // for keys >= ind
            
            if (it->first < (index + segmentSize)) {
                if (it->second.second == false) {
                    // erase, so skip - don't add to new map, unless it is the last key
                    if (std::next(it) == change_log.end()) {
                        // special case when the key is the last in the map
                        int new_key = it->first - (it->first - index);
                        change_log_temp.insert({{new_key, {it->second.first - segmentSize, it->second.second}}});
                    }
                } else { // it->second.second == true
                    if (it->first + (segments_log.find(it->first)->second).size() <= (index + segmentSize)) {
                        int to_erase = it->first;
                        //change_log.erase(to_erase); // just don't add it to the change_log_temp
                        segments_log.erase(to_erase);
                    } else {
                        // update change_log: only erase the first (ind + shift) - key) elements
                        change_log_temp.insert({{index /* OR it->first - shift OR min(ind,it->first) ? */, {0, true}}});
                        
                        // update segments_log
                        std::vector<std::byte> segm_to_merge;
                        segm_to_merge = segments_log.find(it->first)->second;
                        auto it_till_erase = segm_to_merge.begin() + (index + segmentSize - it->first);
                        segm_to_merge.erase(segm_to_merge.begin(), it_till_erase);
                        segments_log.find(it->first)->second = segm_to_merge;
                        
                        auto key_to_shift = segments_log.extract(it->first);
                        key_to_shift.key() = index /* OR it->first - shift OR min(ind,it->first) ? */;
                        segments_log.insert(move(key_to_shift));
                    }
                }
            } else { // it->first >= (index + segmentSize): (key - shift, val - shift)
                // update change_log
                
                if ((it->first == (index + segmentSize) && ((std::prev(it))->second.second == true))) {
                    // merge with prev
                    std::vector<std::byte> segm_merged = segments_log.find((std::prev(it))->first)->second;
                    std::vector<std::byte> segm_to_merge = segments_log.find(it->first)->second;
                    segm_merged.insert(segm_merged.end(), segm_to_merge.begin(), segm_to_merge.end());
                    segments_log.find((std::prev(it))->first)->second = segm_merged;
                    
                    int to_erase = it->first;
                    change_log.erase(to_erase);
                    segments_log.erase(to_erase);
                    
                } else {
                    int updated_val = (it->second.second == true) ? 0 : (it->second.first - segmentSize);
                    change_log_temp.insert({{(it->first - segmentSize), {updated_val, it->second.second}}});
                    
                    // update segments_log
                    if (it->second.second == true) {
                        auto key_to_shift = segments_log.extract(it->first);
                        key_to_shift.key() = it->first - segmentSize;
                        segments_log.insert(move(key_to_shift));
                    }
                }
            }
            
        }
        
        change_log = change_log_temp;
        
        // the following is to merge any insertions that are one after another
        for (auto it = change_log.begin(); it != change_log.end(); ++it) {
            if (it->second.second == true) {
                if (std::next(it) != change_log.end() && (std::next(it))->second.second == true) {
                    std::cout << "merging two inserts" << std::endl;
                    auto next_it = std::next(it);
                    std::vector<std::byte> segm_merged = segments_log.find(it->first)->second;
                    std::vector<std::byte> segm_to_merge = segments_log.find(next_it->first)->second;
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
            empty = true;
            change_log[0] = {0, false};
        }
    }
    
}

void TetianaGenome::insert(size_t index, const std::vector<std::byte>& segment) {
    
    if (empty == true) empty = false;
    
    std::cout << "change_log.size(): " << change_log.size() << std::endl;
    
    std::cout << "insert" << std::endl;
    
    int segmentSize = segment.size();
    std::cout << "segmentSize" << segmentSize << std::endl;
    
    auto next_key = change_log.upper_bound(index); // can be end()
    auto lb_key = change_log.lower_bound(index); // can be end()
    auto prev_key = std::prev(change_log.upper_bound(index)); // always valid, as change_log.size() >= 1
    
    std::cout << "next_key: " << next_key->first << ": " << next_key->second.first << ", " << next_key->second.second << std::endl;
    std::cout << "lb_key: " << lb_key->first << ": " << lb_key->second.first << ", " << lb_key->second.second << std::endl;
    std::cout << "prev_key: " << prev_key->first << ": " << prev_key->second.first << ", " << prev_key->second.second << std::endl;
    
    if (lb_key == change_log.end()) {
        // the last key will always be false, so if there nothing <= than index: just add index to both maps
        
        // update change_log
        change_log.insert({{index, {0, true}}});
        change_log.insert({{index + segmentSize, {segmentSize, false}}});
        
        // update segments_log
        segments_log.insert({{index, segment}});
                
        return;
    }
    
//    if (next_key == change_log.end()) {
//        // next_key will only be end() if
//        // lb_key is also end() - took care of this case before
//        // lb_key is not end(): this means that ind is the last key in the map => last key in the map will always be false, so this will be taken care of in the "else" condition
//    }
    
    std::map<int, std::pair<int, bool>> change_log_temp;
    
    std::cout << "prev_key->second.second == true" << std::endl;
    std::cout << prev_key->second.second << std::endl;
    std::cout << "next_key != change_log.end() && next_key->second.second == true" << std::endl;
    std::cout << (next_key != change_log.end() && next_key->second.second == true) << std::endl;
    
    if (prev_key->second.second == true) {
        std::cout << "prev_key->second.second == true" << std::endl;
        // update change_log
        for (auto it = change_log.begin(); it != std::next(prev_key); ++it) {
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        for (auto it = std::next(prev_key); it != change_log.end(); ++it) {
            change_log_temp.insert(std::make_pair(it->first + segmentSize, std::make_pair(it->second.first + segmentSize, it->second.second)));
        }
        
        // update segments_log
        auto it_to_merge = segments_log.find(prev_key->first); // find is O(1) here
        std::vector<std::byte> segm_to_merge;
        if (it_to_merge != segments_log.end()) {
            segm_to_merge = it_to_merge->second;
            segm_to_merge.insert(segm_to_merge.begin() + (index - prev_key->first), segment.begin(), segment.end());
            // TODO: what is segm_to_merge.begin() + (ind - prev_key->first) is our of range?
            it_to_merge->second = segm_to_merge; // replace with merged segment in segments_log
        } else {
            // should not happen, because prev_key->second.second == true
            std::cout << "SEGMENTS MAP IS WRONG!" << std::endl;
        }
    } else if (next_key != change_log.end() && next_key->second.second == true) {
        if (lb_key->first != index) {
            // update change_log
            for (auto it = change_log.begin(); it != prev_key; ++it) {
                change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
            }
            
            change_log_temp.insert(std::make_pair(prev_key->first, std::make_pair(index - prev_key->second.first, prev_key->second.second)));
            
            for (auto it = next_key; it != change_log.end(); ++it) {
                int updated_val = (it->second.second == true) ? 0 : (it->second.first + segmentSize);
                change_log_temp.insert(std::make_pair(it->first + segmentSize, std::make_pair(updated_val, it->second.second)));
                
                // update segments_log
                if (it->second.second == true) {
                    auto key_to_shift = segments_log.extract(it->first);
                    key_to_shift.key() = it->first + segmentSize;
                    segments_log.insert(move(key_to_shift));
                }
            }
            change_log_temp[index] = {0, true}; // want to replace if exists
            
            //change_log_temp.insert({{index + segmentSize, {next_key->first - index, false}}});
            change_log_temp.insert({{index + segmentSize, {prev_key->second.first + segmentSize, false}}});
            
            // update segments_log
            segments_log.insert({{index, segment}});
        } else { // lb_key->first == index

            // update change_log
            for (auto it = change_log.begin(); it != lb_key; ++it) { // is lb_key==prev_key always?
                change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
            }
            for (auto it = lb_key; it != change_log.end(); ++it) {
                int updated_val = (it->second.second == true) ? 0 : (it->second.first + segmentSize);
                change_log_temp.insert(std::make_pair(it->first + segmentSize, std::make_pair(updated_val, it->second.second)));
                
                // update segments_log
                if (it->second.second == true) {
                    auto key_to_shift = segments_log.extract(it->first);
                    key_to_shift.key() = it->first + segmentSize;
                    segments_log.insert(move(key_to_shift));
                }
            }
            //change_log_temp.insert({{ind, {0, true}}});
            change_log_temp[index] = {0, true}; // want to replace if exists
            
            // update segments_log
            segments_log.insert({{index, segment}});
            
        }
    } else {
        // no insertions before nor after ind
        // also takes care of "next_key == change_log.end()"
        // update change_log
        for (auto it = change_log.begin(); it != lb_key; ++it) {
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        for (auto it = lb_key; it != change_log.end(); ++it) {
            int updated_val = (it->second.second == true) ? 0 : (it->second.first + segmentSize);
            change_log_temp.insert(std::make_pair(it->first + segmentSize, std::make_pair(updated_val, it->second.second)));
            
            // update segments_log
            if (it->second.second == true) {
                auto key_to_shift = segments_log.extract(it->first);
                key_to_shift.key() = it->first + segmentSize;
                segments_log.insert(move(key_to_shift));
            }
        }
        //change_log_temp.insert(std::make_pair(index, std::make_pair(0, false))); // Jul 13: why false?
        change_log_temp.insert(std::make_pair(index, std::make_pair(0, true)));
        
        // also add shift just after insertion, e.g. "Insert(2, {1})" on slides
        if ((index + segmentSize) <= next_key->first) { //next_key == end() already checked
            change_log_temp[index + segmentSize] = {segmentSize, false};
        }
        
        // update segments_log
        segments_log.insert(std::make_pair(index, segment));
    }
    
    
    change_log = change_log_temp; // is this constant time?
    
    // the following is to merge any insertions that are one after another
    for (auto it = change_log.begin(); it != change_log.end(); ++it) {
        if (it->second.second == true) {
            if (std::next(it) != change_log.end() && (std::next(it))->second.second == true) {
                std::cout << "merging two inserts" << std::endl;
                auto next_it = std::next(it);
                std::vector<std::byte> segm_merged = segments_log.find(it->first)->second;
                std::vector<std::byte> segm_to_merge = segments_log.find(next_it->first)->second;
                segm_merged.insert(segm_merged.end(), segm_to_merge.begin(), segm_to_merge.end());
                segments_log.find(it->first)->second = segm_merged; // rempace in segments_log
                
                int to_erase = next_it->first;
                change_log.erase(to_erase);
                segments_log.erase(to_erase);
            }
        }
    }
    
}

std::vector<std::byte> TetianaGenome::offspring_recon() {
    
//    std::cout << " in TetianaGenome::offspring_recon " << std::endl;
    
    if (empty == true) {
        return std::vector<std::byte>{};
    }
    
    // if (change_log.empty()) return sites; // it's never empty
    if (change_log.size() == 1) {
        if (change_log.begin()->first == 0 && change_log.begin()->second.first == 0 && change_log.begin()->second.second == false) {
//            std::cout << "===== sites_offspring =====" << std::endl;
//            for(int i = 0; i < sites.size(); ++i) {
//                std::cout << (int)sites[i] << " ";
//            }
//            std::cout << std::endl;
//            std::cout << "===========================" << std::endl;

            return sites; // no mutations
        }
    }
    
    // Calculte size of the offspring genome .rbegin() will not return .rend(), because log is not empty
    int offspr_sites_size = sites.size() + change_log.rbegin()->second.first; // last key's val
    std::vector<std::byte> sites_offspring(offspr_sites_size, (std::byte)0);
    
    for (auto it_curr = change_log.begin(); it_curr != change_log.end(); ++it_curr) {
        auto it_next = std::next(it_curr);
        
        if (it_curr->second.second == true) {
            std::vector<std::byte> vals_to_insert = segments_log.at(it_curr->first);
            if (it_next == change_log.end()) { // will this ever happen?
                // This loop fills out the last part of the vector
                std::cout << "offspr_sites_size - it_curr->first " << (offspr_sites_size - it_curr->first) << std::endl;
                int ind_to_insert = 0;
                for (int ind = it_curr->first; ind < offspr_sites_size; ++ind) {
                    sites_offspring[ind] = vals_to_insert[ind_to_insert];
                    ind_to_insert++;
                }
            } else {
                std::cout << "it_next->first - it_curr->first " << (it_next->first - it_curr->first) << std::endl;
                int ind_to_insert = 0;
                for (int ind = it_curr->first; ind < it_next->first; ++ind) {
                    std::cout << "ind: " << ind << ", vals_to_insert[ind_to_insert]: " << (int)vals_to_insert[ind_to_insert] << std::endl;
                    sites_offspring[ind] = vals_to_insert[ind_to_insert];
                    ind_to_insert++;
                }
            }
        } else {
            if (it_next == change_log.end()) {
                // This loop fills out the last part of the vector
                for (int ind = it_curr->first; ind < offspr_sites_size; ++ind) {
                    int ind_orig = ind - it_curr->second.first;
                    sites_offspring[ind] = sites[ind_orig];
                }
            } else {
                for (int ind = it_curr->first; ind < it_next->first; ++ind) {
                    int ind_orig = ind - it_curr->second.first;
                    sites_offspring[ind] = sites[ind_orig];
                }
            }
        }
    }
    
//    std::cout << "===== sites_offspring =====" << std::endl;
//    for(int i = 0; i < sites_offspring.size(); ++i) {
//        std::cout << (int)sites_offspring[i] << " ";
//    }
//    std::cout << std::endl;
//    std::cout << "===========================" << std::endl;

    
    return sites_offspring;
}

void TetianaGenome::show() {
    
    std::vector<std::byte> offspring_sites = offspring_recon();
    
    for (auto& s : offspring_sites) {
        std::cout << (int)s << " ";
    }
    std::cout << std::endl;
}


//GeneView TetianaGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}

