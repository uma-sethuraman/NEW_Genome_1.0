#include "TetianaGenome.h"
#include <iostream>

TetianaGenome::TetianaGenome(size_t _size): AbstractGenome(_size),sites(_size){
    sites.resize(_size);
}

std::byte* TetianaGenome::data(size_t index, size_t byteSize) {
    std::vector<std::byte> offspring_sites = offspring_recon();
    
    return static_cast<std::byte*>(&offspring_sites[index]); // cast is for demonstration
}

void TetianaGenome::resize(size_t new_size) {
    size_ = new_size;
};

void TetianaGenome::insert(size_t index, std::vector<std::byte> segment) {
    
    int shift = segment.size();
    
    // Keys after this one will be affected by insertion
    auto next_key_it = change_log.lower_bound(index);
    
    if (next_key_it == change_log.end()) {
        auto curr_key_it = change_log.insert(std::make_pair(index, std::make_pair(0, true)));
        segments_log.insert(std::make_pair(index, segment));
        
        // Update the indexes after insertion
        if (curr_key_it.first == change_log.begin()) // map was empty just before the previous line
            change_log.insert(std::make_pair(index + shift, std::make_pair(shift, false)));
        else
            change_log.insert(std::make_pair(index + shift, std::make_pair((--curr_key_it.first)->second.first + shift, false)));
        
    } else {
        
        // I don't think it is possible to loop through the map and update the keys (iterators get invalidated)
        // Therefore, create new map with updated keys
        std::map<int, std::pair<int, bool>> change_log_temp;
        
        // Insert all keys < ind into new map
        for (std::map<int, std::pair<int, bool>>::iterator it = change_log.begin(); it != next_key_it; ++it) {
            //std::cout << "inserting key: " << it->first << ", value: " << it->second.first << std::endl;
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        
        // Insert all keys >= ind into new map
        for (std::map<int, std::pair<int, bool>>::iterator it = next_key_it; it != change_log.end(); ++it) {
            //std::cout << "shift: " << shift << std::endl;
            //std::cout << "inserting key: " << it->first + shift << ", value: " << it->second.first + shift << std::endl;
            if (it->second.second == true) {
                change_log_temp.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first, it->second.second)));
                
                // also need to update segments_log
                auto key_to_shift = segments_log.extract(it->first);
                key_to_shift.key() = it->first + shift;
                segments_log.insert(move(key_to_shift));
            } else {
                change_log_temp.insert(std::make_pair(it->first + shift, std::make_pair(it->second.first + shift, it->second.second)));
            }
        }
        
        // the following can be optimized, as it repeats the code in "if"
        auto curr_key_it = change_log_temp.insert(std::make_pair(index, std::make_pair(0, true)));
        
        if (curr_key_it.first == change_log_temp.begin()) {
            // first inserted element - nothing else to check
            // Add inserted values to the unordered_map
            segments_log.insert(std::make_pair(index, segment));
            // In this case (insertion index is largest key in the map), there will never be a situation when the previous key is also insersion, because insertion is always followed bu the key shifted by the size of insertion.
        } else {
            // Check if previous key is insertion
            auto prev_key_it = std::prev(curr_key_it.first);
            if (prev_key_it->second.second == true) {
                // insertion
                if ((curr_key_it.first->first - prev_key_it->first) > shift) {
                    // don't change previous - they don't overlap
                    // Add inserted values to the unordered_map
                    segments_log.insert(std::make_pair(index, segment));
                } else {
                    // overlap with previous insertion (TODO replace copying)
                    std::vector<std::byte> segment_temp = segments_log.at(prev_key_it->first);
                    auto temp_shift = curr_key_it.first->first - prev_key_it->first;
                    segment_temp.insert(segment_temp.begin() + temp_shift, segment.begin(), segment.end());
                    change_log_temp.erase(index);
                }
            }
        }
        
        change_log = change_log_temp;
        
    }
    
}

std::vector<std::byte> TetianaGenome::offspring_recon() {
    
    if (change_log.empty()) return sites; // no mutations
    
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

