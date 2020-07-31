/**
 * \file UmaGenome.cpp
 * \author Uma Sethuraman
 **/

#include "UmaGenome.h"
#include <iostream>
#include <vector>
#include <random>
#include <memory>

UmaGenome::UmaGenome(size_t _size){

    /* constructing a genome from scratch so 
       set parent (shared pointer) to point 
       to empty vector of size _size */
    parent = std::make_shared<std::vector<std::byte>>(_size);

    currentGenomeSize = _size;

    // insert initial default entry into offset map
    offsetMap.insert({0, 0});

    // no mutations have occurred yet, parent genome still intact
    mutationsOccurred = false;
}

size_t UmaGenome::size() {
    return currentGenomeSize;
}

/* returns a pointer to byteSize bytes of data,
   starting at index in the current genome */
std::byte* UmaGenome::data(size_t index, size_t byteSize) {
    if ((index+byteSize) > currentGenomeSize){
        std::cout << "error: byteSize exceeds the size of current genome" << std::endl;
        exit(-1);
    }

    if(!mutationsOccurred) {
        /* before any mutations, don't need to check changelog and offset map.
           return from parent directly. */
        return static_cast<std::byte*>(&(parent->at(index)));
    }
    else{
        /* mutations have occurred so need to
           use changelog and offset map */

        int dataSize = 0; // bytes of data to return
        int end = 0; // end index of data to return
        if (byteSize == 0){
            // get entire genome starting at index
            dataSize = currentGenomeSize;
            end = currentGenomeSize;
        }
        else{
            // get genome from index to index+byteSize(end)
            dataSize = byteSize;
            end = index+byteSize;
        }

        // allocate required amount of data
        std::byte* dataPtr = (std::byte*)malloc(dataSize*sizeof(std::byte));
        if(dataPtr == nullptr) {
            std::cout << "data allocation in data function failed, exiting!" << std::endl;
            exit(-1);
        }

        // get values in range requested by user
        int data_index = 0;
        for (int gen_index = index; gen_index < end && data_index < dataSize; ++gen_index) {
            dataPtr[data_index] = getCurrentGenomeAt(gen_index);
            ++data_index;
        }

        /* add pointer to this genome's dataPointers vector,
           pointer will be deleted in genome's destructor */
        dataPointers.push_back(dataPtr);

        // return pointer to requested data
        return dataPtr;
   }
}

// resizes the current genome to new_size
void UmaGenome::resize(size_t new_size) {
    /* reset the parent to point to the 
       reconstructed current genome and clear
       the changelog and offset map */
    genomeReset();
    
    // resize the vector of parent genome values
    parent->resize(new_size);

    // modify size variable
    currentGenomeSize = new_size;
};

// gets offset of key's lower bound from offsetMap
int UmaGenome::getLowerBoundOffset(int key) {
    std::map<int,int>::iterator it = offsetMap.lower_bound(key);

    // find closest lower bound (closest key less than (key) in map)
    if (it != offsetMap.begin())
        --it;
    int lb_offset = it->second;
    return lb_offset;
}

/* Returns a clone of the current genome.
   If forceCopy is true, then it resets the "parent" genome of the clone
   to point to the actual current genome and clears the changelog and offset map.
   If forceCopy is false, it directly clones the current genome, 
   leaving the parent, changelog, and offset map the same in the clone. */
AbstractGenome* UmaGenome::clone(bool forceCopy) {
    if (forceCopy) {
        // create a clone with an empty changelog and offset map
        AbstractGenome* cloneGenome = new UmaGenome(currentGenomeSize);

        /* reset the parent in the clone to point to 
           the reconstructed current genome */
        (static_cast<UmaGenome*>(cloneGenome))->parent = std::make_shared<std::vector<std::byte>>(getCurrentGenome());
        return cloneGenome;
    }
    else {
        // clone directly (copy over changelog, offset map, and parent)
        return new UmaGenome(*this);
    }
}

// point mutation at position index with given value
void UmaGenome::pointMutate(size_t index, std::byte value) {
    std::map<int, std::byte>::iterator it = changelog.find(index);
    if(it != changelog.end()){
        // key index already in changelog, change its value
        it->second = value;
    }
    else
    {
        // key index not in changelog, insert it with value
        changelog.insert({index, value});
    }

    mutationsOccurred = true;
}

/* overwrite the current genome values at indices 
   from [index, index+segment.size()) with the 
   values from the segment vector */
void UmaGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
    if (index + segment.size() > currentGenomeSize) {
        std::cout << "attempt to overwrite would write past end of genome! exiting..." << std::endl;
        exit(1);
    }

    int size = segment.size();
    int seg_index = 0;

    // calls point mutate on each byte in segment
    for (int gen_index = index; gen_index < index+size; ++gen_index) {
        pointMutate(gen_index, segment[seg_index]);
        ++seg_index;
    }

    mutationsOccurred = true;
}

/* insert the values in the segment vector 
   into the current genome, starting at index */
void UmaGenome::insert(size_t index, const std::vector<std::byte>& segment) {
    if (index > currentGenomeSize) {
        std::cout << "attempt to insert past end of genome! exiting..." << std::endl;
        exit(1);
    }

    int size = segment.size(); // insertion size

    /* Loop from last key in changelog until you hit index (or its upper bound) 
       and increment the keys by insertion size. Incrementing the keys in reverse
       order prevents us from changing a key into a key that already exists */
    std::map<int,std::byte>::reverse_iterator changelog_it = changelog.rbegin();
    while(changelog_it != changelog.rend() && (changelog_it->first >= index)) {

        // increment current key in changelog by size
        int key = changelog_it->first;
        auto keyToChange = changelog.extract(key);
        keyToChange.key() += size;
        changelog.insert(move(keyToChange));
        ++changelog_it;
    }

    // Insert (key, value) pairs from insertion into changelog
    int seg_index = 0;
    for(int insertInd = index; insertInd < (index+size); ++insertInd) {
        changelog.insert({insertInd, segment[seg_index]});
        ++seg_index;
    }

    /* Loop from last key in offset map until you hit index+size (position after
       end of insertion) or its upper bound. Increment the keys by insertion size. 
       Incrementing the keys in reverse order prevents us from changing 
       a key into a key that already exists. */
    std::map<int,int>::reverse_iterator offset_it_rev = offsetMap.rbegin();
    while(offset_it_rev != offsetMap.rend() && (offset_it_rev->first >= (index+size))) {

        // increment current key in offset map by size
        int key = offset_it_rev->first;
        auto keyToChange = offsetMap.extract(key);
        keyToChange.key() += size;
        offsetMap.insert(move(keyToChange));

        // increment current value (offset) by size
        offset_it_rev->second += size;
        ++offset_it_rev;
    }


    /* If position after insertion (index+size) isn't a key in offsetMap, 
       add it to offsetMap */
    std::map<int,int>::iterator offset_it = offsetMap.find(index+size);
    if (offset_it == offsetMap.end()){
        // (index+size) not in offsetMap, so add it
        // set its offset to (offset of lower bound + insertion size)
        int lb_offset = getLowerBoundOffset(index+size);
        offsetMap.insert({index+size, lb_offset+size});
    }

    currentGenomeSize += size; // update current genome size

    mutationsOccurred = true;
}

/* remove segmentSize values from the
   current genome, starting at index */
void UmaGenome::remove(size_t index, size_t segmentSize) {
    if (index + segmentSize > currentGenomeSize) {
        std::cout << "attempt to remove past end of genome! exiting..." << std::endl;
        exit(1);
    }

    // Remove all keys in deletion from changelog
    for (int deleteInd = index; deleteInd < (index+segmentSize); ++deleteInd) {
        // erase key if it's in changelog
        // this call does nothing if key isn't in changelog
        changelog.erase(deleteInd); 
    }

    /* Loop through every position after the end of the deletion in changelog
       and decrement keys by size */
    std::map<int,std::byte>::iterator changelog_it = changelog.upper_bound(index+segmentSize-1);
    while(changelog_it != changelog.end()) {

        // decrement current key in changelog by size
        int key = changelog_it->first;
        auto keyToChange = changelog.extract(key);
        keyToChange.key() -= segmentSize;
        changelog.insert(move(keyToChange));
        ++changelog_it;
    }

    /* Add index after end of deletion (index+segmentSize) to offset map
       if it's not already there. This position is where offsetting begins. */
    std::map<int,int>::iterator offset_it = offsetMap.find(index+segmentSize);
    if (offset_it == offsetMap.end()){
        // index+segmentSize not in offsetMap, so add it
        // set its offset equal to offset of lower bound
        int lb_offset = getLowerBoundOffset(index+segmentSize);
        offsetMap.insert({index+segmentSize, lb_offset});
        offset_it = offsetMap.find(index+segmentSize);
    }

    // Remove all keys in deletion from offset map
    for (int deleteInd = index; deleteInd < (index+segmentSize); ++deleteInd) {
        // erase key if it's in offset map
        // this call does nothing if key isn't in offset map
        offsetMap.erase(deleteInd); 
    }

    /* Decrement all keys/values starting after end of deletion (index+segmentSize)
       in offset map by size because these positions are all 
       offsetted by the deletion */
    while(offset_it != offsetMap.end()) {

        // decrement current key in offset map by size
        int key = offset_it->first;
        auto keyToChange = offsetMap.extract(key);
        keyToChange.key() -= segmentSize;
        offsetMap.insert(move(keyToChange));

        // decrement current value (offset) by size
        offset_it->second -= segmentSize;
        ++offset_it;
    }

    currentGenomeSize -= segmentSize; // update current genome size

    mutationsOccurred = true;
}

// prints entire current genome
void UmaGenome::show() {
    for (int index = 0; index < currentGenomeSize; ++index) {
        std::byte& num = GN::genomeRead<std::byte>(this, index);
        std::cout << (int)num << " ";
    }
    std::cout << std::endl;
}

// prints changelog
void UmaGenome::printChangelog() {
    std::map<int,std::byte>::iterator it;

    for(it = changelog.begin(); it != changelog.end(); ++it)
    {
        std::cout << "Key: " << it->first << ", ";
        std::cout << "Value: " << (int)it->second << std::endl;
    }
}

// prints offset map
void UmaGenome::printOffsetMap() {
    std::map<int,int>::iterator it;

    for(it = offsetMap.begin(); it != offsetMap.end(); ++it)
    {
        std::cout << "Key: " << it->first << ", ";
        std::cout << "Offset: " << it->second << std::endl;
    }
}

// returns a vector of bytes containing the entire current genome
std::vector<std::byte> UmaGenome::getCurrentGenome() {

    // reconstruct current genome using changelog and offset map
    std::vector<std::byte> current(currentGenomeSize);
    for (int ind = 0; ind < currentGenomeSize; ++ind) {
        current[ind] = getCurrentGenomeAt(ind);
    }
    return current;
}

/* resets parent to point to the reconstructed current genome,
   collapses/resets changelog and offset map */
void UmaGenome::genomeReset() {

    // change parent genome to point to reconstructed current genome
    parent = std::make_shared<std::vector<std::byte>>(getCurrentGenome());;
    
    // clear changelog
    changelog.clear();

    // clear offset map and insert default (0,0) entry
    offsetMap.clear();
    offsetMap.insert({0,0});

    // no mutations have occurred yet on the reset genome
    mutationsOccurred = false;
}

// random access function: returns value in current genome at position pos
std::byte UmaGenome::getCurrentGenomeAt(int pos) {
    std::map<int,std::byte>::iterator changelog_it;
    changelog_it = changelog.find(pos);
    if (changelog_it != changelog.end()) {
        // pos is in changelog, just return its value
        return changelog_it->second;
    }
    else {
        // pos not in changelog
        std::map<int,int>::iterator offsetmap_it;
        offsetmap_it = offsetMap.find(pos);
        int pos_offset = 0;
        if (offsetmap_it != offsetMap.end()) {
            // offset map contains pos, so just get offset directly
            pos_offset = offsetmap_it->second;
        }
        else {
            // offset map doesn't contain pos, so must get offset of pos' lower bound
            std::map<int,int>::iterator lb_it;
            lb_it = offsetMap.lower_bound(pos);
            if (lb_it != offsetMap.begin())
                --lb_it;
            pos_offset = lb_it->second;
        }

        // get value from parent at index (pos-pos_offset)
        return parent->at(pos-pos_offset);
    }
}
