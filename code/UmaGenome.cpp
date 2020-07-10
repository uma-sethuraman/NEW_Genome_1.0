#include "UmaGenome.h"
#include <iostream>
#include <vector>
#include <random>
#include "Uma_NKEvaluator.h"
#include "random.h"

UmaGenome::UmaGenome(size_t _size): AbstractGenome(_size),sites(_size){
    sites.resize(_size);

    // insert initial default entry into offset map
    offsetMap.insert({0, 0});

    // no mutations have occurred yet, parent genome intact
    mutationsOccurred = false;
}

// Still need to figure out how to delete pointer returned
std::byte* UmaGenome::data(size_t index, size_t byteSize) {
    if ((index+byteSize) > size_){
        std::cout << "error: byteSize exceeds the size of current genome" << std::endl;
        exit(-1);
    }

    if(!mutationsOccurred) {
        // before any mutations, don't need to check changelog
        return static_cast<std::byte*>(&sites[index]);
    }
    else{
        // need to use changelog
        int vectorSize = 0;
        int end = 0;
        if (byteSize == 0){
            // get entire genome starting at index
            vectorSize = size_;
            end = size_;
        }
        else{
            // get genome from index to index+byteSize
            vectorSize = byteSize;
            end = index+byteSize;
        }

        // create vector with requested data
        std::vector<std::byte> result(vectorSize);
        int res_index = 0;
        for (int gen_index = index; gen_index < end && res_index < vectorSize; gen_index++){
            result[res_index] = getCurrentGenomeAt(gen_index);
            res_index++;  
        }

        return static_cast<std::byte*>(result.data());
   }
}

void UmaGenome::resize(size_t new_size) {
    size_ = new_size;
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

    /* Increment all keys starting at start in changelog by size by 
       looping through changelog in reverse order until we hit loop_end_key 
       - where loop_end_key is start if start is in changelog or 
       start's upper bound otherwise. */
    std::map<int,std::byte>::iterator loop_end = changelog.find(index);
    if(loop_end == changelog.end())
        loop_end = changelog.upper_bound(index);
    int loop_end_key = loop_end->first; // index to stop the loop

    /* Looping in reverse order and incrementing the keys prevents us from
       changing a key into a key that already exists */
    std::map<int,std::byte>::reverse_iterator changelog_it = changelog.rbegin();
    while(changelog_it != changelog.rend() && (changelog_it->first >= loop_end_key)) {

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

    // If insertion start isn't a key in offsetMap, add it to offsetMap
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

    size_ += size; // update current genome size

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

    size_ -= segmentSize; // update current genome size

    mutationsOccurred = true;
}

// prints entire current genome
void UmaGenome::show() {
    for (int index = 0; index < size_; index++) {
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

//GeneView UmaGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
