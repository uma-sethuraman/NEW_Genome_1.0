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
	//if the mutation is within the genome size
	if (index <= size_ + 1) {
		size_t indexCounter = index;
		//loop through the sites that will be inserted
		for (int i = 0; i < segment.size(); i++) {
			//check if the site exists in the changelog
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
			//site does not exist in the changelog
			else {
				ChangelogStruct c = ChangelogStruct();
				c.value = segment[i];
				c.insertOffset = 1;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
				keys.clear();
				std::map<size_t, ChangelogStruct>::iterator site = changelog.find(indexCounter);
				for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
					keys.push_back(it->first);
				}
				keys.erase(keys.begin());
				for (auto rit = keys.rbegin(); rit != keys.rend(); ++rit) {
					auto keyIndex = *rit;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex + (size_t)1;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
			}
			indexCounter++;
		}
	};
}

/*void StephanieGenome::insert(size_t index, const std::vector<std::byte>& segment) {
	printChangelog();
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	for (auto site : changelog) {
		removeOffset += site.second.removeOffset;
		insertOffset += site.second.insertOffset;
	}
	auto size_ = ((size() - removeOffset) + insertOffset) - 1;
	if (index <= size_ + 1) {
		size_t indexCounter = index;
		for (int i = 0; i < segment.size(); i++) {
			std::cout << "\tloop " << i << std::endl;
			//loop through the sites that will be inserted
			//check if the site exists in the changelog
			if (changelog.count(indexCounter)) {
				std::cout << "\t\t" << indexCounter << " exists in changelog" << std::endl;
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
						std::cout << "\t\t\tblablablabla test " << std::endl;
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
				std::cout << "\t\t" << indexCounter << " does not exist in changelog" << std::endl;
				ChangelogStruct c = ChangelogStruct();
				c.value = segment[i];
				c.insertOffset = 1;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
			}
			indexCounter++;
		}
	};
}*/

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	size_t indexRemoved = index;
	std::vector<size_t> removeSites;
	//get the genome size
	//todo could this 'get size_ method'  be improved?
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
			ChangelogStruct c = ChangelogStruct();
			c.removeOffset = segmentSize;
			c.insertOffset = 0;
			c.value = (std::byte)0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		}
		//get the sites that will be removed
		for (size_t i = indexRemoved; i <= removeIndex; i++) {
			removeSites.push_back(indexRemoved);
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
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	size_t index = 0;
	size_t diff = 0;
	for (auto site : changelog) {
		removeOffset += site.second.removeOffset;
		insertOffset += site.second.insertOffset;
	}
	size_t size_ = (size() - removeOffset) + insertOffset;
	std::vector<std::byte> newSites(size_);
	std::cout << std::endl;
	for (int i = 0; i < newSites.size(); i++) {
		//if site does not exist in the changelog
		if (!changelog.count(index)) {
			newSites[i] = sites[index - diff];
			index++;
		}
		//if the site contains an overwrite mutation
		else if (changelog.count(index) && (changelog[index].removeOffset == 0 && changelog[index].insertOffset == 0)) {
			newSites[i] = changelog[index].value;
			index++;
		}
		//if the site contains an insert mutation
		else if (changelog.count(index) && changelog[index].insertOffset > 0) {
			diff += changelog[index].insertOffset;
			//todo need to update
			newSites[i] = changelog[index].value;
			index++;
		}
		//if the site contains an remove mutation
		else if (changelog.count(index) && changelog[index].removeOffset > 0) {
			index += changelog[index].removeOffset;
			if (i == 0) {
				newSites[i] = sites[index];
				if (changelog.count(index)) {
					i += 1;
					newSites[i] = changelog[index].value;
					index++;
				}
			}
			else if (!changelog.count(index)) {
				newSites[i] = sites[index];
			}
			else {
				newSites[i] = changelog[index].value;
			}
			index++;
		}
	}
	for (int i = 0; i < newSites.size(); i++) {
		std::cout << (int)newSites[i] << " ";
	}
	sites.clear();
	sites = newSites;
}

/*void StephanieGenome::show() {
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
	size_t index = 0;
	std::cout << std::endl;
	for (int i = 0; i < newSites.size(); i++) {
		//if site does not exist in the changelog
		if (!changelog.count(i)) {
			newSites[i] = sites[original];
			std::cout << "\tnot in changelog" << std::endl;
			std::cout << "\ti:\t\t\t" << i << std::endl;
			std::cout << "\toriginal:\t\t" << original << std::endl;
			std::cout << "\tsites[original]:\t" << (int)sites[original] << std::endl;
			std::cout << "\tcurrent:\t\t" << current << std::endl;
			std::cout << "\tindex:\t\t\t" << index << std::endl;
			original++;
			current++;
		}
		//if the site contains an overwrite mutation
		else if (changelog.count(i) && (changelog[i].removeOffset == 0 && changelog[i].insertOffset == 0)) {
			newSites[i] = changelog[current].value;
			std::cout << "\toverwrite" << std::endl;
			std::cout << "\ti:\t\t\t" << i << std::endl;
			std::cout << "\toriginal:\t\t" << original << std::endl;
			std::cout << "\tcurrent:\t\t" << current << std::endl;
			std::cout << "\tchangelog[current].value:\t" << (int)changelog[current].value << std::endl;
			std::cout << "\tindex:\t\t\t" << index << std::endl;
			original++;
			current++;
		}
		//if the site contains an insert mutation
		else if (changelog.count(i) && changelog[i].insertOffset > 0) {
			newSites[i] = changelog[current].value;
			std::cout << "\tinsert" << std::endl;
			std::cout << "\ti:\t\t\t" << i << std::endl;
			std::cout << "\toriginal:\t\t" << original << std::endl;
			std::cout << "\tcurrent:\t\t" << current << std::endl;
			std::cout << "\tchangelog[current].value:\t" << (int)changelog[current].value << std::endl;
			std::cout << "\tindex:\t\t\t" << index << std::endl;
			current++;
		}
		//if the site contains an remove mutation
		else if (changelog.count(i) && changelog[i].removeOffset > 0) {
			original += changelog[i].removeOffset;
			std::cout << "\tremove" << std::endl;
			std::cout << "\ti:\t\t\t" << i << std::endl;
			if (changelog.count(original)) {
				newSites[i] = changelog[original].value;
				std::cout << "\toriginal w/ offset:\t" << original << std::endl;
				std::cout << "\tchangelog[original].value:\t" << (int)changelog[original].value << std::endl;
			}
			else {
				newSites[i] = sites[original];
				std::cout << "\toriginal w/ offset:\t" << original << std::endl;
				std::cout << "\tsites[original]:\t" << (int)sites[original] << std::endl;
			}
			std::cout << "\tcurrent:\t\t" << current << std::endl;
			std::cout << "\tindex:\t\t\t" << index << std::endl;
			original++;
			current++;
		}
		std::cout << std::endl;
	}
	for (int i = 0; i < newSites.size(); i++) {
		std::cout << (int)newSites[i] << " ";
	}
	sites.clear();
	sites = newSites;
}*/

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