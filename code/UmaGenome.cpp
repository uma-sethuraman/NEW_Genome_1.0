#include "UmaGenome.h"
#include <iostream>
#include <vector>
#include <random>

UmaGenome::UmaGenome(size_t _size){
    sites.resize(_size);
    currentGenomeSize = _size;

    // insert initial default entry into offset map
    offsetMap.insert({0, 0});

    // no mutations have occurred yet, parent genome intact
    mutationsOccurred = false;
}

size_t UmaGenome::size() {
    return currentGenomeSize;
}

// Still need to figure out how to free pointer returned
std::byte* UmaGenome::data(size_t index, size_t byteSize) {
    if ((index+byteSize) > currentGenomeSize){
        std::cout << "error: byteSize exceeds the size of current genome" << std::endl;
        exit(-1);
    }

    if(!mutationsOccurred) {
        // before any mutations, don't need to check changelog
        return static_cast<std::byte*>(&sites[index]);
    }
    else{
        // need to use changelog
        int dataSize = 0;
        int end = 0;
        if (byteSize == 0){
            // get entire genome starting at index
            dataSize = currentGenomeSize;
            end = currentGenomeSize;
        }
        else{
            // get genome from index to index+byteSize
            dataSize = byteSize;
            end = index+byteSize;
        }

        // allocate required amount of data
        std::byte* ret = (std::byte*)malloc(dataSize*sizeof(std::byte));
        if (ret == nullptr) {
            std::cout << "data allocation failed, exiting" << std::endl;
            exit(-1);
        }

        // set values in range requested by user
        int ret_index = 0;
        for (int gen_index = index; gen_index < end && ret_index < dataSize; gen_index++) {
            ret[ret_index] = getCurrentGenomeAt(gen_index);
            ret_index++;
        }
        return ret;
   }
}

// will resize the sites vector and change current genome size
void UmaGenome::resize(size_t new_size) {
    sites.resize(new_size);
    currentGenomeSize = new_size;
};

// gets offset of key's lower bound from offsetMap
int UmaGenome::getLowerBoundOffset(int key) {
    std::map<int,int>::iterator it = offsetMap.lower_bound(key);

    // find closest lower bound (key that's immediately less than key in map)
    if (it != offsetMap.begin())
        --it;
    int lb_offset = it->second;
    return lb_offset;
}

// Not fully implemented or used yet
void UmaGenome::mutate() {
    // Something like the code below
    /*int numPoint = Random::getBinomial((int)sites.size(), MR_Point);
    int numIns = Random::getBinomial((int)sites.size(), MR_Insertion);
    int numDel = Random::getBinomial((int)sites.size(), MR_Deletion);*/

    /*for (int p = 0; p < numPoint; p++) {
        pointMutate();
    }

    for (int i = 0; i < numIns; i++) {
        insertMutate();
    }

    for (int d = 0; d < numDel; d++) {
        deleteMutate();
    }*/
}

// point mutation at position index with given value
void UmaGenome::pointMutate(size_t index, std::byte value) {
    std::map<int, std::byte>::iterator it = changelog.find(index);
    if(it != changelog.end()){
        // key index already in map, change its value
        it->second = value;
    }
    else
    {
        // key index not in map, insert it with value
        changelog.insert({index, value});
    }

    mutationsOccurred = true;
}

void UmaGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
    int size = segment.size();
    int seg_index = 0;

    // calls point mutate on each byte in segment
    for (int gen_index = index; gen_index < index+size; gen_index++) {
        pointMutate(gen_index, segment[seg_index]);
        seg_index++;
    }

    mutationsOccurred = true;
}

void UmaGenome::insert(size_t index, const std::vector<std::byte>& segment) {

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
        changelog_it++;
    }

    // Insert (key, value) pairs from insertion into changelog
    int seg_index = 0;
    for(int insertInd = index; insertInd < (index+size); insertInd++) {
        changelog.insert({insertInd, segment[seg_index]});
        seg_index++;
    }

    /* Loop from last key in offset map until you hit index (or its upper bound) 
       and increment the keys by insertion size. Incrementing the keys in reverse
       order prevents us from changing a key into a key that already exists */
    std::map<int,int>::reverse_iterator ofs_it = offsetMap.rbegin();
    while(ofs_it != offsetMap.rend() && (ofs_it->first >= index)) {

        // increment current key in offset map by size
        int key = ofs_it->first;
        auto keyToChange = offsetMap.extract(key);
        keyToChange.key() += size;
        offsetMap.insert(move(keyToChange));
        ofs_it++;
    }


    // If insertion start position isn't a key in offsetMap, add it to offsetMap
    std::map<int,int>::iterator offset_it = offsetMap.find(index);
    if (offset_it == offsetMap.end()){
        // start not in offsetMap, so add it
        // set its offset to offset of lower bound
        int lb_offset = getLowerBoundOffset(index);
        offsetMap.insert({index, getLowerBoundOffset(index)});
        offset_it = offsetMap.find(index);
    }

    // Add insertion size to all offsets in offsetMap beginning at start
    while(offset_it != offsetMap.end()) {
        offset_it->second += size;
        offset_it++;
    }

    currentGenomeSize += size; // update current genome size

    mutationsOccurred = true;
}

void UmaGenome::remove(size_t index, size_t segmentSize) {

    // Remove all keys in deletion from changelog
    for (int deleteInd = index; deleteInd < (index+segmentSize); deleteInd++) {
        // erase key if it's in changelog
        // this call does nothing if key isn't in changelog
        changelog.erase(deleteInd); 
    }

    // Loop through every position after the end of the deletion and decrement keys by size
    std::map<int,std::byte>::iterator changelog_it = changelog.upper_bound(index+segmentSize-1);
    while(changelog_it != changelog.end()) {

        // decrement current key in changelog by size
        int key = changelog_it->first;
        auto keyToChange = changelog.extract(key);
        keyToChange.key() -= segmentSize;
        changelog.insert(move(keyToChange));
        changelog_it++;
    }

    // Decrement all keys starting at index in offset map
    std::map<int,int>::iterator ofs_it = offsetMap.upper_bound(index);
    while(ofs_it != offsetMap.end()) {
        int key = ofs_it->first;
        auto keyToChange = offsetMap.extract(key);
        keyToChange.key() -= (segmentSize-1);
        offsetMap.insert(move(keyToChange));
        ofs_it++;
    }

    // If start not a key in offsetMap, add it to offsetMap
    std::map<int,int>::iterator offset_it = offsetMap.find(index);
    if (offset_it == offsetMap.end()){
        // start not in offsetMap, so add it
        // set its offset equal to offset of lower bound
        int lb_offset = getLowerBoundOffset(index);
        offsetMap.insert({index, getLowerBoundOffset(index)});
        offset_it = offsetMap.find(index);
    }

    // Subtract deletion size from all offsets in offsetMap beginning at start
    while(offset_it != offsetMap.end()) {
        offset_it->second -= segmentSize;
        offset_it++;
    }

    currentGenomeSize -= segmentSize; // update current genome size

    mutationsOccurred = true;
}

// prints entire current genome
void UmaGenome::show() {
    for (int index = 0; index < currentGenomeSize; index++) {
        std::byte& num = GN::genomeRead<std::byte>(this, index);
        std::cout << (int)num << " ";
    }
    std::cout << std::endl;
}

// prints changelog
void UmaGenome::printChangelog() {
    std::map<int,std::byte>::iterator it;

    for(it = changelog.begin(); it != changelog.end(); it++)
    {
        std::cout << "Key: " << it->first << ", ";
        std::cout << "Value: " << (int)it->second << std::endl;
    }
}

// prints offset map
void UmaGenome::printOffsetMap() {
    std::map<int,int>::iterator it;

    for(it = offsetMap.begin(); it != offsetMap.end(); it++)
    {
        std::cout << "Key: " << it->first << ", ";
        std::cout << "Offset: " << it->second << std::endl;
    }
}

// returns value in current genome at position pos
// random access function
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
        return sites[pos-pos_offset];
    }
}
