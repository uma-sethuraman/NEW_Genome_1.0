#include "StephanieGenome.h"
#include <iostream>
#include <utility>
#include <map>
#include <typeinfo>
#include <algorithm>
#include <stdlib.h>

StephanieGenome::StephanieGenome(size_t _size) {
	sites.resize(_size);
}

std::byte* StephanieGenome::data(size_t index, size_t byteSize) {
	return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
}

size_t StephanieGenome::size() {
	return sites.size();
}

void StephanieGenome::resize(size_t new_size) {
	sites.resize(new_size);
};

void StephanieGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	for (auto site : changelog) {
		removeOffset += site.second.removeOffset;
		insertOffset += site.second.insertOffset;
	}
	auto size_ = ((size() - removeOffset) + insertOffset) - 1;

	if (index <= size_) {
		auto site = index;
		for (auto seg : segment) {
			if (changelog.count(site)) {
				changelog[site].value = seg;
			}
			else {
				ChangelogStruct c = ChangelogStruct();
				c.value = seg;
				c.insertOffset = 0;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(site, c));
			}
			site++;
		}
	}
}

void StephanieGenome::insert(size_t index, const std::vector<std::byte>& segment) {
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	for (auto site : changelog) {
		removeOffset += site.second.removeOffset;
		insertOffset += site.second.insertOffset;
	}
	auto size_ = ((size() - removeOffset) + insertOffset) - 1;
	if (index <= size_) {
		size_t indexCounter = index;
		for (int i = 0; i < segment.size(); i++) {
			if (changelog.count(indexCounter)) {
				//std::cout << "insert vector: key exists" << std::endl;
				//std::cout << "\tindex " << indexCounter << " value " << (int)values[i] << std::endl;
				if (changelog.count(indexCounter)) {
					keys.clear();
					std::map<size_t, ChangelogStruct>::iterator site = changelog.find(indexCounter);
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
					c.value = segment[i];
					c.insertOffset = 1;
					c.removeOffset = 0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
				}
				else {
					keys.clear();
					if (changelog.count(indexCounter)) {
						//std::cout << " is present " << std::endl;
						std::map<size_t, ChangelogStruct>::iterator site = changelog.begin();
						for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
							//std::cout << "\t" << it->first << ", " << (int)it->second.value << " " << it->second.insertOffset << std::endl;
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
						c.value = segment[i];
						c.insertOffset = 1;
						c.removeOffset = 0;
						changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
					}
					else {
						ChangelogStruct c = ChangelogStruct();
						c.value = segment[i];
						c.insertOffset = 1;
						c.removeOffset = 0;
						changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
					}

				}

			}
			else {
				//std::cout << "insert vector: key does not exist" << std::endl;
				//std::cout << "\tindex " << indexCounter << " value " << (int)values[i] << std::endl;
				ChangelogStruct c = ChangelogStruct();
				c.value = segment[i];
				c.insertOffset = 1;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
			}
			indexCounter++;
		}
	};
}

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	size_t indexRemoved = index;
	std::vector<size_t> removeSites;
	//get the genome size
	//todo could this be improved?
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	for (const auto& site : changelog) {
		removeOffset += site.second.removeOffset;
		insertOffset += site.second.insertOffset;
	}
	auto size_ = ((size() - removeOffset) + insertOffset) - 1;
	size_t removeIndex = index + segmentSize - 1;
	//if the mutation is within the genome size
	if (removeIndex <= size_) {
		//check if the site exists in the changelog
		if (changelog.count(index)) {
			std::cout << "\tsite " << index << " exists" << std::endl;
			//site contains an overwrite mutation
			if (changelog[index].insertOffset == 0 && changelog[index].removeOffset == 0) {
				changelog[index].removeOffset = segmentSize;
				changelog[index].insertOffset = 0;
				changelog[index].value = (std::byte)0;
			}
			//site contains an insert mutation
			else if (changelog[index].insertOffset > 0) {
				changelog.erase(index);
			}
			//site contains a removal mutation
			else if (changelog[index].removeOffset > 0) {
				changelog[index].removeOffset += 1;
			}
		}
		else {
			std::cout << "\tsite " << index << " does not exist" << std::endl;
			ChangelogStruct c = ChangelogStruct();
			c.removeOffset = segmentSize;
			c.insertOffset = 0;
			c.value = (std::byte)0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		}
		//get the sites that will be removed
		for (size_t i = indexRemoved; i <= removeIndex; i++) {
			removeSites.push_back(indexRemoved);
			std::cout << "\tsite to be removed " << indexRemoved << std::endl;
			indexRemoved++;
		}
		//collapse neighboring remove sites
		int exit = 0;
		size_t currentIndex = index;
		removeSites.erase(removeSites.begin());
		for (size_t site = index; site <= removeIndex; site++) {
			if (changelog.count(currentIndex + 1) && (changelog[currentIndex + 1].removeOffset > 0) && changelog[currentIndex].removeOffset > 0) {
				changelog[currentIndex].removeOffset += changelog[currentIndex + 1].removeOffset;
				changelog.erase(currentIndex + 1);
				std::vector<size_t>::iterator itr = std::find(removeSites.begin(), removeSites.end(), currentIndex + 1);
				if (removeSites.size() == 1) {
					removeSites.pop_back();
				}
				else {
					removeSites.erase(itr);
					for (int i = 0; i < removeSites.size(); i++) {
						auto remainingSite = removeSites[i];
						auto nh = changelog.extract(remainingSite);
						nh.key() = remainingSite - 1;
						changelog.insert(move(nh));
					}
				}
			}
		}
	}
}

void StephanieGenome::show() {
	printChangelog();
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	for (auto site : changelog) {
		removeOffset += site.second.removeOffset;
		insertOffset += site.second.insertOffset;
	}
	size_t size_ = (size() - removeOffset) + insertOffset;
	std::vector<std::byte> newSites;
	newSites.resize(size_);
	size_t original = 0;
	size_t current = 0;
	std::cout << std::endl;
	for (int i = 0; i < newSites.size(); i++) {
		//if site does not exist in the changelog
		if (!changelog.count(i)) {
			newSites[i] = sites[original];
			original++;
			current++;
		}
		//if the site contains an overwrite mutation
		else if (changelog.count(i) && (changelog[i].removeOffset == 0 && changelog[i].insertOffset == 0)) {
			newSites[i] = changelog[current].value;
			original++;
			current++;
		}
		//if the site contains an insert mutation
		else if (changelog.count(i) && changelog[i].insertOffset > 0) {
			newSites[i] = changelog[current].value;
			current++;
		}
		//if the site contains an remove mutation
		else if (changelog.count(i) && changelog[i].removeOffset > 0) {
			original += changelog[i].removeOffset;
			if (changelog.count(original)) {
				newSites[i] = changelog[original].value;
			}
			else {
				newSites[i] = sites[original];
			}
			original++;
			current++;
		}

	}
	for (int i = 0; i < newSites.size(); i++) {
		std::cout << (int)newSites[i] << " ";
	}
	sites.clear();
	sites = newSites;
}

void StephanieGenome::printChangelog() {
	std::cout << "\n--- start of map ---" << std::endl;
	for (auto site : changelog) {
		std::cout << site.first;
		std::byte siteValue = site.second.value;
		size_t removeOffset = site.second.removeOffset;
		size_t insertOffset = site.second.insertOffset;
		printf(", value: %i ", siteValue);
		printf(" removeOffset: %i ", removeOffset);
		printf(" insertOffset: %i", insertOffset);
		std::cout << std::endl;
	}
	std::cout << "--- end of map ---" << std::endl << std::endl;
}

//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}