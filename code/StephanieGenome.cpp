#include "StephanieGenome.h"
#include <iostream>
#include <typeinfo>
#include <stdlib.h>

//TODO: implement printing all sites from original genome and modified genome
/*
	loop through original genome
	if site exists in map
	modify offset get value
*/
//TODO: implement random access - aka can you get to any point in the genome
//TODO: improve code, make it more readable and reduce the memory/performance
// :~)

StephanieGenome::StephanieGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* StephanieGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void StephanieGenome::resize(size_t new_size) {
	size_ = new_size;
};

void StephanieGenome::insertMutation(size_t index, std::vector<std::byte> values) {
	size_t indexCounter = index;
	for (int i = 0; i < values.size(); i++) { 
		if (changelog.count(indexCounter)) {
			//std::cout << "insert vector: key exists" << std::endl;
			//std::cout << "\tindex " << indexCounter << " value " << (int)values[i] << std::endl;
			insertMutation(indexCounter, values[i]);
		}
		else {
			//std::cout << "insert vector: key does not exist" << std::endl;
			//std::cout << "\tindex " << indexCounter << " value " << (int)values[i] << std::endl;
			ChangelogStruct c = ChangelogStruct();
			c.value = values[i];
			c.offset = 1;
			changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
		}
		indexCounter++;
	}
}

void StephanieGenome::insertMutation(size_t index, std::byte value) {
	if (changelog.count(index)) {
		//std::cout << "insert: key exists" << std::endl;
		//std::cout << "\tindex " << index << " value " << (int)value << std::endl;
		keys.clear();
		std::map<size_t, ChangelogStruct>::iterator site = changelog.find(index);
		for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
			keys.push_back(it->first);
		}
		for (auto rit = keys.rbegin(); rit != keys.rend(); ++rit) {
			auto keyIndex = *rit;
			auto nh = changelog.extract(keyIndex);
			keyIndex = keyIndex + (size_t)1;
			nh.key() = keyIndex;
			changelog.insert(move(nh));
		}
		ChangelogStruct c = ChangelogStruct();
		c.value = value;
		c.offset = 1;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
	else { 
		//std::cout << "insert: key does not exist" << std::endl;
		//std::cout << "\tindex " << index << " value " << (int)value << std::endl;
		ChangelogStruct c = ChangelogStruct();
		c.value = value;
		c.offset = 1;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
}

void StephanieGenome::deleteMutation(size_t index, int siteOffset) {
	if (changelog.count(index)) {
		//std::cout << "delete: key exists" << std::endl;
		//std::cout << "\tindex " << index << " siteOffset " << siteOffset << std::endl;
		for (int i = 0; i < abs(siteOffset); i++) {
			keys.clear();
			std::map<size_t, ChangelogStruct>::iterator site = changelog.find(index);
			//std::cout << "\t(" << index << ") sites added to key vector  ";
			for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
				keys.push_back(it->first);
				//std::cout << it->first << " ";
			}
			//std::cout << std::endl;
			changelog.erase(index);
			keys.erase(keys.begin());
			for (auto it = keys.begin(); it != keys.end(); ++it) {
				auto keyIndex = *it;
				auto nh = changelog.extract(keyIndex);
				keyIndex = keyIndex - (size_t)1;
				nh.key() = keyIndex;
				changelog.insert(move(nh));
			}
		}
	}
	else {
		//std::cout << "delete: key does not exist" << std::endl;
		//std::cout << "\tindex " << index << " siteOffset " << siteOffset << std::endl;
		ChangelogStruct c = ChangelogStruct();
		c.offset = siteOffset;
		c.value = (std::byte)0;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
	auto affectedSites = index + 1;
	for (int i = 0; i < (abs(siteOffset) - 1); i++) {
		if (changelog.count(affectedSites) && changelog[affectedSites].offset < 0) {
			//std::cout << "\t(" << index << ") " << " site affected " << affectedSites << " has deletion node of " << changelog[affectedSites].offset << std::endl;
			changelog[index].offset = changelog[index].offset + changelog[affectedSites].offset;
			changelog.erase(affectedSites);
		}
		affectedSites++;
	}
	for (auto site : changelog) {
		if (changelog.count(site.first + abs(siteOffset)) && changelog[site.first + abs(siteOffset)].offset < 0) {
			auto nextSite = site.first + abs(siteOffset);
			//std::cout << "\t(" << site.first << ") " << "next site " << nextSite << " has deletion " << changelog[nextSite].offset << std::endl;
			if ((nextSite - 1) == site.first) {
				changelog[site.first].offset = changelog[index].offset + changelog[nextSite].offset;
				changelog.erase(nextSite);
			}
		}
	}
}

void StephanieGenome::pointMutation(size_t index, std::byte value) {
	if(changelog.count(index)) {
		changelog[index].value = value;
	}
	else {
		ChangelogStruct c = ChangelogStruct();
		c.value = value;
		c.offset = 0;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
}

void StephanieGenome::printChangelog() {
	std::cout << "\n--- start of map ---" << std::endl;
	for (auto site : changelog) {
		std::cout << site.first;
		std::byte siteValue = site.second.value;
		int siteOffset = site.second.offset;
		printf(", value: %i ", siteValue);
		printf(" siteOffset: %i\n", siteOffset);
	}
	std::cout << "--- end of map ---\n" << std::endl;
}

void StephanieGenome::generateGenome(AbstractGenome* genome) {
	int sum = 0;
	for (auto site : changelog) {
		auto offset = site.second.offset;
		//std::cout << "offset: " << offset << std::endl;
		sum = sum + offset;
	}
	//std::cout << "sum: " << sum << std::endl;
	//std::cout << "genome size: " << genome->size() << std::endl;
	sum = genome->size() + sum;
	std::vector<std::byte> modifiedSites;
	modifiedSites.resize(sum);

	size_t genomeIndex = 0;
	size_t modifiedIndex = 0;
	for (int i = 0; i < modifiedSites.size(); i++) {
		std::cout << std::endl;
		if (!changelog.count(i)) { //if genomeIndex is not on map
			std::cout << i << " is not on map" <<  std::endl;
			std::cout << "i: " << i << std::endl;
			std::cout << "genome index: " << genomeIndex << std::endl;
			std::cout << "modified index: " << modifiedIndex << std::endl;
			std::cout << "value: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, genomeIndex);
			genomeIndex++;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].offset == 0) { //point mutation
			std::cout << i << " is a point mutation" << std::endl;
			std::cout << "i: " << i << std::endl;
			std::cout << "genome index: " << genomeIndex << std::endl;
			std::cout << "modified index: " << modifiedIndex << std::endl;
			std::cout << "value: " << (int)changelog[genomeIndex].value << std::endl;
			modifiedSites[i] = changelog[modifiedIndex].value;
			genomeIndex++;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].offset > 0) { //insert mutation
			std::cout << i << " is a insert mutation" << std::endl;
			std::cout << "i: " << i << std::endl;
			std::cout << "genome index: " << genomeIndex << std::endl;
			std::cout << "modified index: " << modifiedIndex << std::endl;
			std::cout << "value: " << (int)changelog[modifiedIndex].value << std::endl;
			modifiedSites[i] = changelog[modifiedIndex].value;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].offset < 0) { //delete mutation
			std::cout << i << " is a delete mutation" << std::endl;
			std::cout << "i: " << i << std::endl;
			std::cout << "offset: " << abs(changelog[i].offset) << std::endl;
			genomeIndex = genomeIndex + abs(changelog[i].offset);
			std::cout << "genome index: " << genomeIndex << std::endl;
			std::cout << "modified index: " << modifiedIndex << std::endl;
			std::cout << "value: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, genomeIndex);
			genomeIndex++;
			modifiedIndex++;
		}
	}



/*
	if (!changelog.count(i)) { //if the site is not on the map
		std::cout << "i: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
		i++;
		genomeIndex++;
		modifiedIndex++;
	}
	else if (changelog.count(i) && changelog[i].offset == 0) { //point mutation
		std::cout << "i: " << (int)changelog[modifiedIndex].value << std::endl;
		i++;
		genomeIndex++;
		modifiedIndex++;
	}
	else if (changelog.count(i) && changelog[i].offset > 0) { //insert mutation
		std::cout << "i: " << (int)changelog[modifiedIndex].value << std::endl;
		i++;
		modifiedIndex++;
	}
*/


/*	for (int i = 0; i < modifiedSites.size(); i++) {
		if (!changelog.count(i)) {
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, i);
		}
		else if(changelog.count(i) && changelog[i].value != (std::byte)0) {
			modifiedSites[i] = changelog[i].value;
		}
		else if (changelog.count(i) && changelog[i].value == (std::byte)0) {

		}
	}*/

	for (int i = 0; i < modifiedSites.size(); i++) {
		std::cout << (int)modifiedSites[i] << " ";
	}
	std::cout << std::endl << std::endl;
}



//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
