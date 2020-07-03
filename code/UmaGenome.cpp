#include "UmaGenome.h"
#include <iostream>
#include <vector>
#include <random>
#include "Uma_NKEvaluator.h"
#include "random.h"

UmaGenome::UmaGenome(size_t _size): AbstractGenome(_size),sites(_size){
    sites.resize(_size);
    alphabetSize = 0;
    currentGenomeSize = _size;

    // insert initial default entry into offset map
    offsetMap.insert({0, 0});
}

UmaGenome::UmaGenome(size_t _size, int alph_size): AbstractGenome(_size),sites(_size) {
    sites.resize(_size);
    alphabetSize = alph_size;
    currentGenomeSize = _size;

    // insert initial default entry into offset map
    offsetMap.insert({0, 0});
}

std::byte* UmaGenome::data() {
    return static_cast<std::byte*>(sites.data()); // cast is for demonstration
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
void UmaGenome::pointMutate(int index, std::byte value) {
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
}

// insert mutation at index start of vector values
void UmaGenome::insertMutate(int start, std::vector<std::byte> values) {

    int size = values.size(); // insertion size

    /* Increment all keys starting at start in changelog by size by 
       looping through changelog in reverse order until we hit loop_end_key 
       - where loop_end_key is start if start is in changelog or 
       start's upper bound otherwise. */
    std::map<int,std::byte>::iterator loop_end = changelog.find(start);
    if(loop_end == changelog.end())
        loop_end = changelog.upper_bound(start);
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
    int values_index = 0;
    for(int insertInd = start; insertInd < (start+size); insertInd++) {
        changelog.insert({insertInd, values[values_index]});
        values_index++;
    }

    // If insertion start isn't a key in offsetMap, add it to offsetMap
    std::map<int,int>::iterator offset_it = offsetMap.find(start);
    if (offset_it == offsetMap.end()){
        // start not in offsetMap, so add it
        // set its offset to offset of lower bound
        int lb_offset = getLowerBoundOffset(start);
        offsetMap.insert({start, getLowerBoundOffset(start)});
        offset_it = offsetMap.find(start);
    }

    // Add insertion size to all offsets in offsetMap beginning at start
    while(offset_it != offsetMap.end()) {
        offset_it->second += size;
        offset_it++;
    }

    currentGenomeSize += size; // update current genome size
}

// delete mutation at index start of given size
void UmaGenome::deleteMutate(int start, int size) {

    // Remove all keys in deletion from changelog
    for (int deleteInd = start; deleteInd < (start+size); deleteInd++) {
        // erase key if it's in changelog
        // this call does nothing if key isn't in changelog
        changelog.erase(deleteInd); 
    }

    // Loop through every position after the end of the deletion and decrement keys by size
    std::map<int,std::byte>::iterator changelog_it = changelog.upper_bound(start+size-1);
    while(changelog_it != changelog.end()) {

        // decrement current key in changelog by size
        int key = changelog_it->first;
        auto keyToChange = changelog.extract(key);
        keyToChange.key() -= size;
        changelog.insert(move(keyToChange));
        changelog_it++;
    }

    // If start not a key in offsetMap, add it to offsetMap
    std::map<int,int>::iterator offset_it = offsetMap.find(start);
    if (offset_it == offsetMap.end()){
        // start not in offsetMap, so add it
        // set its offset equal to offset of lower bound
        int lb_offset = getLowerBoundOffset(start);
        offsetMap.insert({start, getLowerBoundOffset(start)});
        offset_it = offsetMap.find(start);
    }

    // Subtract deletion size from all offsets in offsetMap beginning at start
    while(offset_it != offsetMap.end()) {
        offset_it->second -= size;
        offset_it++;
    }

    currentGenomeSize -= size; // update current genome size
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

// prints out entire current genome
void UmaGenome::reconstructGenome() {
    for (int pos = 0; pos < currentGenomeSize; pos++) {
        std::byte val = getCurrentGenomeAt(pos);
        std::cout << (int)val << " ";
    }
    std::cout << std::endl;
}

//GeneView UmaGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
