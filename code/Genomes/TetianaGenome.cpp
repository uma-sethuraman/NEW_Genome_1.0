#include "Genomes/TetianaGenome.h"
#include <iostream>

TetianaGenome::TetianaGenome(size_t _size) : genomeSize(_size) {
    sites.resize(_size);
}

size_t TetianaGenome::size() {
    return genomeSize;
}

std::byte* TetianaGenome::data(size_t index, size_t byteSize) {

    bool mutations = false;
    
    if (!(change_log.size() == 1 && (change_log.begin()->first == 0 && change_log.begin()->second.first == 0 && change_log.begin()->second.second == false))) {
        
        mutations = true;
    }
    
    if (mutations == false) {
        return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
    } else {
        std::vector<std::byte> offspring_sites = offspring_recon();
        return static_cast<std::byte*>(&offspring_sites[index]); // cast is for demonstration
    }
}

void TetianaGenome::resize(size_t new_size) {
    sites.resize(new_size);
    
    // TODO: this has to be rewritten
    change_log.clear();
    change_log[0] = {0, false};
    segments_log.clear();
    
    genomeSize = new_size;
};

AbstractGenome* TetianaGenome::clone(bool forceCopy) {
    return new TetianaGenome(*this); // TODO: implement this
}

int TetianaGenome::get_ind(int ind_curr) {
    auto it_curr = --change_log.upper_bound(ind_curr);
    if (it_curr->second.second == true) { // insertion
        // TODO: do I really need this check?
        std::cout << "get_ind doesn't work for inserted values" << std::endl;
        return -1;
    }
    else { // deletion
        return ind_curr - it_curr->second.first;
    }
}


// starting at index, write values in segement over values currently in genome
void TetianaGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {

    for (int ind = 0; ind < segment.size(); ++ind) {
                
        auto it_next = change_log.upper_bound(ind + index);
        auto it_prev = std::prev(it_next);

        if (it_prev->second.second == true) {
            (segments_log[it_prev->first])[(index + ind) - it_prev->first] = segment[ind];
        } else {
            // update in parent genome
            int ind_orig = get_ind((ind + index));
            sites[ind_orig] = segment[ind];
        }
    }
    
}

void TetianaGenome::remove(size_t index, size_t segmentSize) {
    
    if (index + segmentSize > genomeSize) {
        std::cout << "attept to erase would erase past end of genome! exiting..." << std::endl;
        exit(1);
    }
    
    // Calculate new genome size
    genomeSize -= segmentSize;
    
    if (change_log.lower_bound(index) == change_log.end()) {
        // no key is >= ind -> just add the key and shift
        auto prev_key = std::prev(change_log.lower_bound(index)); // always valid, as change_log.lower_bound(ind) == change_log.end()
        int shift_upd = prev_key->second.first - segmentSize;
        change_log[index] = std::make_pair(shift_upd, false);
    } else {
        auto next_key_it = change_log.lower_bound(index);
        std::map<int, std::pair<int, bool>> change_log_temp;
        
        for (auto it = change_log.begin(); it != next_key_it; ++it) {
            // for keys < ind: keep without change
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
        }
        
        if (next_key_it != change_log.begin() && (std::prev(next_key_it))->second.second == true) {
            // update segments_log: need to remove couple of values at the end of last insertion
            std::vector<std::byte> segm_to_update;
            segm_to_update = segments_log.find((std::prev(next_key_it))->first)->second;
            auto it_from_erase = segm_to_update.end() - (next_key_it->first - index);
            auto it_till_erase = it_from_erase + segmentSize;
            int dist = std::distance(it_till_erase, segm_to_update.end());
            segm_to_update.erase(it_from_erase, (dist <= 0 ? segm_to_update.end() : it_till_erase));
            segments_log.find((std::prev(next_key_it))->first)->second = segm_to_update;
        }
        
        for (auto it = next_key_it; it != change_log.end(); ++it) {
            // for keys >= ind
            if (it->first <= (index + segmentSize)) {
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
            } else { // it->first > (index + segmentSize): (key - shift, val - shift)

                // update change_log
                if ((it->first == (index + segmentSize) && ((std::prev(it))->second.second == true))) {
                    // merge with prev
                    std::cout << (std::prev(it))->first << std::endl;
                    
                    auto it_found = segments_log.find((std::prev(it))->first);
                    if (it_found == segments_log.end()) std::cout << "not found" << std::endl;
                                        
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
                
        if (change_log_temp.size() == 1 && change_log_temp.begin()->first == 0 && (change_log_temp.begin()->second.first + change_log.rbegin()->second.first) <= -sites.size()) {
            // this means that everything was deleted
            genomeSize = 0;
            change_log = change_log_temp;
            change_log[0] = {0, false};
            return;
        }
        
        change_log = change_log_temp;
        
        // Merge any insertions that are one after another
        for (auto it = change_log.begin(); it != change_log.end(); ++it) {
            if (it->second.second == true) {
                if (std::next(it) != change_log.end() && (std::next(it))->second.second == true) {
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
    }
}

void TetianaGenome::insert(size_t index, const std::vector<std::byte>& segment) {

    if (index > genomeSize) {
        std::cout << "index is out of genome's bounds! exiting..." << std::endl;
        exit(1);
    }
    
    int segmentSize = segment.size();
    genomeSize += segmentSize;
    
    auto next_key = change_log.upper_bound(index); // can be end()
    auto lb_key = change_log.lower_bound(index); // can be end()
    auto prev_key = std::prev(change_log.upper_bound(index)); // always valid, as change_log.size() >= 1
        
    if (lb_key == change_log.end()) {
        // the last key will always be false, so if there nothing <= than index: just add index to both maps
        // and add shift of the segment size to teh change_log map
        
        // update change_log
        change_log.insert({{index, {0, true}}});
        change_log.insert({{index + segmentSize, {prev_key->second.first + segmentSize, false}}});

        // update segments_log
        segments_log.insert({{index, segment}});
                
        return;
    }
    
    // Temporary maps that will be updated
    // Need to use temporary maps, because as I iterate through map to update it,
    // the iterators become invalid
    std::map<int, std::pair<int, bool>> change_log_temp;
    std::unordered_map<int, std::vector<std::byte>> segments_log_temp;

    if (prev_key->second.second == true) { // prev_key == lb_key is key == index
        
        // Keys before insertion
        for (auto it = change_log.begin(); it != std::next(prev_key); ++it) { // lb_key instead of std::next(prev_key)?
            // update change_log
            change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
            
            // update segments_log
            if (it->second.second == true) {
                segments_log_temp.insert({{it->first, segments_log.find(it->first)->second}});
            }
        }
        
        // Keys after insertion
        for (auto it = std::next(prev_key); it != change_log.end(); ++it) {
            // update change_log
            int updated_val = (it->second.second == true) ? 0 : (it->second.first + segmentSize);
            change_log_temp.insert(std::make_pair(it->first + segmentSize, std::make_pair(updated_val/*it->second.first + segmentSize*/, it->second.second))); // Jul 22
            
            // update segments_log
            if (it->second.second == true) {
                segments_log_temp.insert({{it->first + segmentSize, segments_log.find(it->first)->second}});
            }
        }

        segments_log = segments_log_temp; // Could this be replaced by iterating backwards?
        
        // Insertion itself
        // No need to add index to change_log, because index will be merged with prev key
        // update segments_log
        // Because prev_key is insertion, we don't insert index directly, but instead merge the segments
        auto it_to_merge = segments_log.find(prev_key->first); // find is O(1) here
        std::vector<std::byte> segm_to_merge;
        if (it_to_merge != segments_log.end()) {
            segm_to_merge = it_to_merge->second;
            segm_to_merge.insert(segm_to_merge.begin() + (index - prev_key->first), segment.begin(), segment.end()); // this is slow for small vectors
            // what is segm_to_merge.begin() + (index - prev_key->first) is our of range? -> should not happen for correct maps
            it_to_merge->second = segm_to_merge; // replace with merged segment in segments_log
        } else {
            // should not happen, because prev_key->second.second == true
            std::cout << "SEGMENTS MAP IS WRONG!" << std::endl;
        }
        
    } else { //if (next_key != change_log.end() && next_key->second.second == true) {

        //if (lb_key->first != index) { // always the case, lb_key->first == index is is "if"

            // Before index
            for (auto it = change_log.begin(); it != lb_key; ++it) { // why prev_key and not lb_key?
                change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
                if (it->second.second == true) {
                    segments_log_temp.insert({{it->first, segments_log.find(it->first)->second}});
                }
            }

            // After index
            for (auto it = next_key; it != change_log.end(); ++it) { // next_key == lb_key when lb_key->first != index; always true?
                int updated_val = (it->second.second == true) ? 0 : (it->second.first + segmentSize);
                change_log_temp.insert(std::make_pair(it->first + segmentSize, std::make_pair(updated_val, it->second.second)));
                
                // update segments_log
                if (it->second.second == true) {
                    segments_log_temp.insert({{it->first + segmentSize, segments_log.find(it->first)->second}});
                }
            }

            change_log_temp[index] = {0, true}; // want to replace if exists
            //change_log_temp.insert({{index + segmentSize, {prev_key->second.first + segmentSize, false}}});
            change_log_temp[index + segmentSize] = {prev_key->second.first + segmentSize, false};

            segments_log_temp.insert({{index, segment}});
            
            // in this situation two inserted segments will never have to merge

            segments_log = segments_log_temp;
    }
    change_log = change_log_temp; // is this constant time? would std::move make it constant time?
}

std::vector<std::byte> TetianaGenome::offspring_recon() {
        
    if (genomeSize == 0) {
        return std::vector<std::byte>{};
    }
    
    if (change_log.size() == 1) {
        if (change_log.begin()->first == 0 && change_log.begin()->second.first == 0 && change_log.begin()->second.second == false) {
            return sites; // no mutations
        }
    }
    
    // Calculte size of the offspring genome .rbegin() will not return .rend(), because log is never empty
    int offspr_sites_size = sites.size() + change_log.rbegin()->second.first; // last key's val
    if (offspr_sites_size != genomeSize) {
        std::cout << "GENOME SIZE IS WRONG!" << std::endl;
        exit(1);
    }
    std::vector<std::byte> sites_offspring(genomeSize, (std::byte)0);
    
    for (auto it_curr = change_log.begin(); it_curr != change_log.end(); ++it_curr) {
        auto it_next = std::next(it_curr);
        
        if (it_curr->second.second == true) {
            std::vector<std::byte> vals_to_insert = segments_log.at(it_curr->first);
            if (it_next == change_log.end()) { // will this ever happen?
                // This loop fills out the last part of the vector
                int ind_to_insert = 0;
                for (int ind = it_curr->first; ind < genomeSize; ++ind) {
                    sites_offspring[ind] = vals_to_insert[ind_to_insert];
                    ind_to_insert++;
                }
            } else {
                int ind_to_insert = 0;
                for (int ind = it_curr->first; ind < it_next->first; ++ind) {
                    sites_offspring[ind] = vals_to_insert[ind_to_insert];
                    ind_to_insert++;
                }
            }
        } else {
            if (it_next == change_log.end()) {
                // This loop fills out the last part of the vector
                for (int ind = it_curr->first; ind < genomeSize; ++ind) {
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
