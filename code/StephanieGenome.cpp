#include "StephanieGenome.h"
#include <iostream>
#include <typeinfo>
#include <stdlib.h>

StephanieGenome::StephanieGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* StephanieGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void StephanieGenome::resize(size_t new_size) {
	size_ = new_size;
};

//TODO: implement printing all sites from original genome and modified genome
//TODO: implement random access - aka can you get to any point in the genome
//TODO: improve code, make it more readable and reduce the memory/performance
// :~)

void StephanieGenome::insertMutation(size_t index, std::vector<std::byte> values) {
	size_t indexCounter = index;
	for (int i = 0; i < values.size(); i++) { 
		if (changelog.count(indexCounter)) {
			std::cout << "insert vector: key exists" << std::endl;
			std::cout << "\tindex " << indexCounter << " value " << (int)values[i] << std::endl;
			insertMutation(indexCounter, values[i]);
		}
		else {
			std::cout << "insert vector: key does not exist" << std::endl;
			std::cout << "\tindex " << indexCounter << " value " << (int)values[i] << std::endl;
			ChangelogStruct c = ChangelogStruct();
			c.value = values[i];
			c.delta = 1;
			changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
		}
		indexCounter++;
	}
}

void StephanieGenome::insertMutation(size_t index, std::byte value) {
	if (changelog.count(index)) {
		std::cout << "insert: key exists" << std::endl;
		std::cout << "\tindex " << index << " value " << (int)value << std::endl;
		std::vector<size_t> keys;
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
		c.delta = 1;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
	else { 
		std::cout << "insert: key does not exist" << std::endl;
		std::cout << "\tindex " << index << " value " << (int)value << std::endl;
		ChangelogStruct c = ChangelogStruct();
		c.value = value;
		c.delta = 1;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
}

void StephanieGenome::deleteMutation(size_t index, int delta) {
	if (changelog.count(index)) {
		std::cout << "delete: key exists" << std::endl;
		std::cout << "\tindex " << index << " delta " << delta << std::endl;
		auto testest = index;
		for (int i = 0; i < abs(delta); i++) {
			std::vector<size_t> keys;
			std::map<size_t, ChangelogStruct>::iterator site = changelog.find(index);
			std::cout << "\t(" << index << ") sites added to key vector  ";
			for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
				keys.push_back(it->first);
				std::cout << it->first << " ";
			}
			std::cout << std::endl;
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
		std::cout << "delete: key does not exist" << std::endl;
		std::cout << "\tindex " << index << " delta " << delta << std::endl;
		ChangelogStruct c = ChangelogStruct();
		c.delta = delta;
		c.value = (std::byte)0;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
	auto testing = index + 1;
	for (int i = 0; i < (abs(delta) - 1); i++) {
		if (changelog.count(testing)) {
			if (changelog[testing].delta < 0) {
				std::cout << "\t(" << index << ") " << " site affected " << testing << " has deletion node of " << changelog[testing].delta << std::endl;
				changelog[index].delta = changelog[index].delta + changelog[testing].delta;
				changelog.erase(testing);
			}
		}
		testing++;
	}
	for (auto element : changelog) {
		if (changelog.count(element.first + abs(delta))) {
			auto test = element.first + abs(delta);
			if (changelog[test].delta < 0) {
				std::cout << "\t(" << element.first << ") " << "next site " << test << " has deletion " << changelog[test].delta << std::endl;
				changelog[element.first].delta = changelog[index].delta + changelog[test].delta;
				changelog.erase(test);
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
		c.delta = 0;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
	}
}

void StephanieGenome::printChangelog() {
	std::cout << "\n--- start of map ---" << std::endl;
	for (auto element : changelog) {
		std::cout << element.first;
		std::byte v = element.second.value;
		int d = element.second.delta;
		printf(", value: %i ", v);
		printf(" delta: %i\n", d);
	}
	std::cout << "--- end of map ---\n" << std::endl;
}

//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
