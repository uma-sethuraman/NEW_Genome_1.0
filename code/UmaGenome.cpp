#include "UmaGenome.h"
#include <iostream>
#include <vector>
#include <random>
#include "Uma_NKEvaluator.h"
#include "random.h"

UmaGenome::UmaGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);

	// set mutation rates and alphabetSize by default
	MR_Point = 0;
	MR_Insertion = 0;
	MR_Deletion = 0;
	alphabetSize = 0;
	currentGenomeSize = _size;

	// insert initial default entry into changelog
	changelog.insert({0, {.site_value = (std::byte)(0), .offset = 0, .has_value = false}});
}

UmaGenome::UmaGenome(size_t _size, double MR_P, double MR_I, double MR_D, int alph_size): AbstractGenome(_size),sites(_size) {
	sites.resize(_size);

	// set mutation rates and alphabetSize
	MR_Point = MR_P;
	MR_Insertion = MR_I;
	MR_Deletion = MR_D;
	alphabetSize = alph_size;
	currentGenomeSize = _size;

	// insert initial default entry into changelog
	changelog.insert({0, {.site_value = (std::byte)(0), .offset = 0, .has_value = false}});
}

std::byte* UmaGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void UmaGenome::resize(size_t new_size) {
	size_ = new_size;
};

// Not fully implemented or used yet
void UmaGenome::mutate() {
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

// Adds changelog entry to map with given key, site_value, and offset of closest lower bound entry
// CLB means closest lower bound
void UmaGenome::addChangelogEntryCLB(int key, int site_val, bool has_val) {
	std::map<int,site_info>::iterator it = changelog.lower_bound(key);

	// Find closest lower bound (key that's immediately less than key in map)
    if (it != changelog.begin())
        --it;
	int clb_offset = it->second.offset;
	changelog.insert({key, {.site_value = (std::byte)(site_val), .offset = clb_offset, .has_value = has_val}});
}

// point mutation at position index with given value
void UmaGenome::pointMutate(int index, int value, bool use_params) {

	// if use_params = true, use parameters passed in for index and value
	// else randomize index and value
	int random_index = getRandomInt(0, currentGenomeSize-1);
	int random_value = getRandomInt(0, alphabetSize-1);
	if (use_params){
		random_index = index;
		random_value = value;
	}

	std::map<int,site_info>::iterator it;
	it = changelog.find(random_index);

	if (it != changelog.end()){
		// if index already in map, modify value
		it->second.site_value = (std::byte)(random_value);
		it->second.has_value = true;
	}
	else {
		// index keynot in map
		// add new entry to map with default CLB offset
		addChangelogEntryCLB(random_index, random_value, true);
	}
}

// NOT IMPLEMENTED YET
void UmaGenome::insertMutate() {
	
}

void UmaGenome::deleteMutate(int start, int size, bool use_params) {

	// used if we have to randomize starting position and size
	int maxSizeDel = 1;
	int minSizeDel = 1;

	// size and start position are either random OR passed in by user
	// based on use_params
	int deleteSize = getRandomInt(minSizeDel, maxSizeDel);
	int deleteStart = getRandomInt(0, currentGenomeSize-deleteSize);
	if(use_params) {
		deleteSize = size;
		deleteStart = start;
	}

	// Loop through all indices in deletion
	std::map<int,site_info>::iterator it;
	for (int index = deleteStart; index < deleteStart + deleteSize; index++) {
		it = changelog.find(index);
		if (it == changelog.end()){
			// if key index not in map, add it
			addChangelogEntryCLB(index, 0, false);
			it = changelog.find(index);

			// only decrement offset if it's the first element in deletion chunk
			if (index == deleteStart)
				it->second.offset -= deleteSize;
		}
		else
		{
			// key already in map, just need to decrement its offset
			it->second.offset -= deleteSize;
		}
	}

	it++; // move iterator to key right after end of deletion chunk

	// iterate over all keys in map after end of deletion chunk
	// change their offsets by size of deletion
	while(it != changelog.end()) {
		it->second.offset -= deleteSize;
		it++;
	}
		
	currentGenomeSize -= deleteSize; // update current genome size
}

// prints changelog
void UmaGenome::printChangelog() {
	std::map<int,site_info>::iterator it;

	for(it = changelog.begin(); it != changelog.end(); it++)
	{
		std::cout << "Key: " << it->first << ", ";
		site_info info = it->second;
		if(!info.has_value)
			std::cout << "Value: NONE, ";
		else
			std::cout << "Value: " << (int)info.site_value << ", ";
		std::cout << "Offset: " << info.offset << std::endl;
	}
}

// returns site_value in current genome at position pos
int UmaGenome::getCurrentGenomeAt(int pos) {
	int value = 0;
	std::map<int,site_info>::iterator it;
	it = changelog.find(pos);

	if(it != changelog.end()) {
		// key pos is in map
		site_info info = it->second;

		if(info.has_value && info.offset == 0){
			// if key pos has value in map and has no offset, return value
			value = (int)info.site_value;
			return value;
		}
		else{
			// key pos has offset, so check key at [pos-offset]
			std::map<int,site_info>::iterator itr = changelog.find(pos-info.offset);
			if (itr != changelog.end()){
				// key at [pos-info.offset] is present in changelog
				site_info map_site = itr->second;
				if (map_site.has_value){
					// if there is a value at key [pos-info.offset] in map
					// that means this value was changed before so return it
					value = (int)map_site.site_value;
					return value;
				}
				else{
					// no value at key [pos-info.offset] in changelog,
					// so get value from parent 
					value = (int)(sites[pos-info.offset]);
					return value;
				}
			}
			else{
				// key at [pos - info.offset] not present in changelog
				// so get value from parent
				value = (int)(sites[pos-info.offset]);
				return value;
			}
		}
	}
	else {
		// key pos not in map
		// so get lower bound of key pos
		std::map<int,site_info>::iterator itr = changelog.lower_bound(pos);

		// get lower bound offset
		if (itr != changelog.begin())
			--itr;
		int clb_offset = itr->second.offset;
		int parent_index = pos - clb_offset;

		itr = changelog.find(parent_index);
		if (itr != changelog.end()){
			// key at [pos-clb_offset] in map
			site_info map_site = itr->second;
			if (map_site.has_value){
				// key at [pos-clb_offset] has value, so return value
				value = (int)map_site.site_value;
				return value;
			}
			else{
				// key at [pos-clb_offset] has no value, get value from parent
				value = (int)(sites[parent_index]);
				return value;
			}
		}
		else {
			// key at [pos-clb_offset] not in map
			// so get value from parent
			value = (int)(sites[parent_index]);
			return value;
		}
	}
}

// prints out entire current genome
void UmaGenome::reconstructGenome() {
	for (int pos = 0; pos < currentGenomeSize; pos++) {
		int val = getCurrentGenomeAt(pos);
		std::cout << val << " ";
	}
	std::cout << std::endl;
}

//GeneView UmaGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
