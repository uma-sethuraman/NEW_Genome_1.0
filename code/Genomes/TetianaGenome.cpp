#include "Genomes/TetianaGenome.h"
#include <iostream>

TetianaGenome::TetianaGenome(size_t _size) : genomeSize(_size) {
    //sites.resize(_size, (std::byte)0); // is it (std::byte)1 by default?
    sites.resize(_size); // all zeros?
    
    change_log.clear();
    change_log.insert({{0, {0, false}}});
    
    segments_log.clear();
    
}

size_t TetianaGenome::size() {
    return genomeSize;
}

std::byte* TetianaGenome::data(size_t index, size_t byteSize) {
    
    // bool mutations = false;
    
    if (change_log.size() == 1 && (change_log.begin()->first == 0 && change_log.begin()->second.first == 0 && change_log.begin()->second.second == false)) {
        
        // no mutations
        return static_cast<std::byte*>(&sites[index]);
    }
    if (byteSize == 1) {
        std::byte* val = new std::byte(val_at_index(index));
        return val;
    }
    
    std::vector<std::byte> offspring_sites = offspring_recon(index, byteSize);
    std::byte* offspring_sites_heap = new std::byte[offspring_sites.size()];
    offspring_sites_heap = offspring_sites.data();
    
    for (int i = 0; i < offspring_sites.size(); ++i) {
        offspring_sites_heap[i] = offspring_sites[i];
    }
    
    //    std::cout << "offspring_sites_heap[i]: " << std::endl;
    //    for (int i = 0; i < offspring_sites.size(); ++i) {
    //        std::cout << (int)offspring_sites_heap[i] << " ";
    //    }
    //    std::cout << std::endl;
    
    
    return offspring_sites_heap;
}

int TetianaGenome::get_ind(int ind_curr) {
    // Get the index in the parent genome, which correspond to the index in the offspring genome
    // This funciton is only called for remove mutations, because the inserted indexes are not the in the oarent
    
    // Find key, which is <= index
    auto it_curr = --change_log.upper_bound(ind_curr);
    return ind_curr - it_curr->second.first;
}

std::byte TetianaGenome::val_at_index(int ind_curr) {
    // Find key, which is <= index
    auto it_curr = --change_log.upper_bound(ind_curr);
    
    if (it_curr->second.second == true) { // was inserted
        int inserted_ind = ind_curr - it_curr->first;
        return (segments_log.at(it_curr->first))[inserted_ind];
    }
    
    // was removed
    int parent_ind = ind_curr - it_curr->second.first; // = get_ind(int ind_curr);
    return sites[parent_ind];
}

void TetianaGenome::resize(size_t new_size) {
    // TODO: it doesn't have to be collapsed
    
    std::vector<std::byte> offspring_sites = offspring_recon(0, 0);
    sites = offspring_sites;
    
    sites.resize(new_size);
    
    change_log.clear();
    change_log[0] = {0, false};
    segments_log.clear();
    
    genomeSize = new_size;
};

AbstractGenome* TetianaGenome::clone(bool forceCopy) {
    // TODO: I have no idea what I'm doing here
    
    if (forceCopy == true) {
        std::vector<std::byte> offspring_sites = offspring_recon(0, 0);
        
        // Genome clone
        // Will this init empty change_log and segments_log?
        AbstractGenome* cloned = new TetianaGenome(genomeSize);
        (static_cast<TetianaGenome*>(cloned))->sites = offspring_sites;
        
        return cloned;
    }
    
    return new TetianaGenome(*this);
}


void TetianaGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
    
    for (int ind = 0; ind < segment.size(); ++ind) {
        
        // TODO: this is not optimal, because O(logN) every iteration
        auto it_next = change_log.upper_bound(ind + index); // O(logN)
        auto it_prev = std::prev(it_next); // O(1)
        
        if (it_prev->second.second == true) {
            // update in segments_log
            (segments_log[it_prev->first])[(index + ind) - it_prev->first] = segment[ind]; // O(1)
        } else {
            // update in parent genome
            // TODO: this is actually wrong, parent genome has to stay intact
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
    
    // Special case, where the segment is inserted at the end
    if (change_log.lower_bound(index) == change_log.end()) {
        // no key is >= ind -> just add the key and shift
        auto prev_key = std::prev(change_log.lower_bound(index)); // always valid for change_log.lower_bound(ind) == change_log.end()
        int shift_upd = prev_key->second.first - segmentSize;
        change_log[index] = std::make_pair(shift_upd, false);
        
        
//        std::cout << "+++++ change_log: +++++" << std::endl;
//        for (auto it = change_log.begin(); it != change_log.end(); ++it) {
//            std::cout << it->first << " : " <<
//            it->second.first << " : " << it->second.second << std::endl;
//        }
//        std::cout << "+++++ segments_log: +++++:" << std::endl;
//        for (auto it = segments_log.begin(); it != segments_log.end(); ++it) {
//            std::cout << it->first << " : ";
//            for (auto v : it->second) {
//                std::cout << (int)v << " ";
//            }
//            std::cout << std::endl;
//        }
//
        
        return;
    }
    
    auto lb_it = change_log.lower_bound(index); // >= index
    auto lb_segm_it = change_log.lower_bound(index + segmentSize); // >= index
//    std::cout << "lb_it->first " << lb_it->first << std::endl;
//    std::cout << "lb_segm_it->first " << lb_segm_it->first << std::endl;

    
    if (lb_it != change_log.begin() && (std::prev(lb_it))->second.second == true) {
        // update segments_log: need to remove a couple of values at the end of last insertion
        std::vector<std::byte> segm_to_update;
        segm_to_update = segments_log.find((std::prev(lb_it))->first)->second; // O(1)
        auto it_from_erase = segm_to_update.end() - (lb_it->first - index);
        auto it_till_erase = it_from_erase + segmentSize;
        int dist = std::distance(it_till_erase, segm_to_update.end());
        segm_to_update.erase(it_from_erase, (dist <= 0 ? segm_to_update.end() : it_till_erase));
        segments_log.find((std::prev(lb_it))->first)->second = segm_to_update; // O(1)?
    }
    
    
    
    
    std::map<int, std::pair<int, bool>> change_log_temp;
    
    for (auto it = change_log.begin(); it != lb_it; ++it) {
        // for keys < ind: keep without change
        change_log_temp.insert(std::make_pair(it->first, std::make_pair(it->second.first, it->second.second)));
    }
    
    
    
    for (auto it = lb_it; it != lb_segm_it; ++it) {
        if (std::next(it) != lb_segm_it) {
            // erase (don't add to change_log_temp)
            if (it->second.second == true) {
                //erase from segments_log
                segments_log.erase(it->first);
            }
        } else {
            if ((std::next(it))->first == (index + segmentSize)) {
                // erase (don't add to change_log_temp)
                if (it->second.second == true) {
                    //erase from segments_log
                    segments_log.erase(it->first);
                }
                continue;
            }
            
            if (it->second.second == false) {
                change_log_temp[index] = {it->second.first - segmentSize, false};
            } else {
                change_log_temp[index] = {0, true};
                
                // update segments_log
                std::vector<std::byte> segm_to_update;
                segm_to_update = segments_log.find(it->first)->second;
                auto it_till_erase = segm_to_update.begin() + (index + segmentSize - it->first);
                segm_to_update.erase(segm_to_update.begin(), it_till_erase);
                segments_log.find(it->first)->second = segm_to_update;
                
                auto key_to_shift = segments_log.extract(it->first);
                if (!key_to_shift.empty()) { // TODO: add this check everywhere
                    key_to_shift.key() = index;
                    segments_log.insert(move(key_to_shift));
                }
            }
        }
    }
    
    for (auto it = lb_segm_it; it != change_log.end(); ++it) {
        
        // update change_log
        int updated_val = (it->second.second == true) ? 0 : (it->second.first - segmentSize);
//        std::cout << "inserting " << it->first - segmentSize << ",  " << updated_val << " : " << it->second.second << std::endl;
        change_log_temp.insert({{(it->first - segmentSize), {updated_val, it->second.second}}});
        
        // update segments_log
        if (it->second.second == true) {
            auto key_to_shift = segments_log.extract(it->first);
            if (!key_to_shift.empty()) {
                key_to_shift.key() = it->first - segmentSize;
                segments_log.insert(move(key_to_shift));
            }
        }
    }
    
//    std::cout << "+++++ change_log before: +++++" << std::endl;
//    for (auto it = change_log.begin(); it != change_log.end(); ++it) {
//        std::cout << it->first << " : " <<
//        it->second.first << " : " << it->second.second << std::endl;
//    }
    
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
    
//        if (change_log.empty() || (change_log.begin())->first != 0) {
//            change_log.insert({{0, {0, false}}});
//        }
    
    
    
//    std::cout << "+++++ change_log: +++++" << std::endl;
//    for (auto it = change_log.begin(); it != change_log.end(); ++it) {
//        std::cout << it->first << " : " <<
//        it->second.first << " : " << it->second.second << std::endl;
//    }
//    std::cout << "+++++ segments_log: +++++:" << std::endl;
//    for (auto it = segments_log.begin(); it != segments_log.end(); ++it) {
//        std::cout << it->first << " : ";
//        for (auto v : it->second) {
//            std::cout << (int)v << " ";
//        }
//        std::cout << std::endl;
//    }
//
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
        
        
//        std::cout << "+++++ change_log: +++++" << std::endl;
//        for (auto it = change_log.begin(); it != change_log.end(); ++it) {
//            std::cout << it->first << " : " <<
//            it->second.first << " : " << it->second.second << std::endl;
//        }
//        std::cout << "+++++ segments_log: +++++:" << std::endl;
//        for (auto it = segments_log.begin(); it != segments_log.end(); ++it) {
//            std::cout << it->first << " : ";
//            for (auto v : it->second) {
//                std::cout << (int)v << " ";
//            }
//            std::cout << std::endl;
//        }
        
        
        return;
    }
    
    // Temporary maps that will be updated
    // Need to use temporary maps, because as I iterate through map to update it,
    // the iterators become invalid
    // TODO: use reverse iterators
    std::map<int, std::pair<int, bool>> change_log_temp;
    std::unordered_map<int, std::vector<std::byte>> segments_log_temp;
    
    if (prev_key->second.second == true) { // prev_key == lb_key if key == index
        
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
        change_log_temp[index + segmentSize] = {prev_key->second.first + segmentSize, false};
        segments_log_temp.insert({{index, segment}});
        
        // in this situation two inserted segments will never have to merge
        segments_log = segments_log_temp;
    }
    change_log = change_log_temp; // is this constant time? would std::move make it constant time?
    
//    std::cout << "+++++ change_log: +++++" << std::endl;
//    for (auto it = change_log.begin(); it != change_log.end(); ++it) {
//        std::cout << it->first << " : " <<
//        it->second.first << " : " << it->second.second << std::endl;
//    }
//    std::cout << "+++++ segments_log: +++++:" << std::endl;
//    for (auto it = segments_log.begin(); it != segments_log.end(); ++it) {
//        std::cout << it->first << " : ";
//        for (auto v : it->second) {
//            std::cout << (int)v << " ";
//        }
//        std::cout << std::endl;
//    }
}

std::vector<std::byte> TetianaGenome::offspring_recon(size_t index, size_t byteSize) {
    
    if (genomeSize == 0) {
        return std::vector<std::byte>{};
    }
    
    int sites_offspring_size = 0;
    if (byteSize == 0) sites_offspring_size = genomeSize;
    else sites_offspring_size = byteSize;
    
    std::vector<std::byte> sites_offspring(genomeSize, (std::byte)0);
    
    for (auto it_curr = change_log.begin(); it_curr != change_log.end(); ++it_curr) {
        auto it_next = std::next(it_curr);
        
        if (it_curr->second.second == true) {
            std::vector<std::byte> vals_to_insert = segments_log.at(it_curr->first);
            
            int ind_to_insert = 0;
            for (int ind = it_curr->first; ind < it_next->first; ++ind) {
                sites_offspring[ind] = vals_to_insert[ind_to_insert];
                ind_to_insert++;
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
    
    // TODO: This is not optimal, but we are not using these in tests yet
    auto start = sites_offspring.begin() + index;
    auto end = sites_offspring.begin() + (index + sites_offspring_size);
    std::vector<std::byte> sites_offspring_part(start, end);
    
    return sites_offspring_part;
}

void TetianaGenome::show() {
    
    // TODO: Is it better to instead call val_at_ind that collapse the genome?
    
    if (change_log.size() == 1 && (change_log.begin()->first == 0 && change_log.begin()->second.first == 0 && change_log.begin()->second.second == false)) {
        // no mutations
        for (auto& s : sites) {
            std::cout << (int)s << " ";
        }
    } else {
        std::vector<std::byte> offspring_sites = offspring_recon(0, 0);
        for (auto& s : offspring_sites) {
            std::cout << (int)s << " ";
        }
    }
    std::cout << std::endl;
}
