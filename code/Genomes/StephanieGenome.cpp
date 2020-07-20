#include "StephanieGenome.h"
#include <iostream>
#include <utility>
#include <map>
#include <typeinfo>
#include <algorithm>
#include <stdlib.h>

StephanieGenome::StephanieGenome(size_t _size) {
	sites.resize(_size);
	genomeSize = _size;
}

std::byte* StephanieGenome::data(size_t index, size_t byteSize) {
	if (!mutationFlag) {
		return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
	}
	else {
		std::vector<std::byte> newSites = generateNewGenome();
		return static_cast<std::byte*>(&newSites[index]);
	}
}

size_t StephanieGenome::size() {
	return genomeSize;
}

void StephanieGenome::resize(size_t new_size) {
	sites.resize(new_size);
	genomeSize = new_size;
};

void StephanieGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
	if (index <= genomeSize - 1) {
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
	mutationFlag = true;
}

void StephanieGenome::insert(size_t index, const std::vector<std::byte>& segment) {
	//if the mutation is within the genome size
	if (index <= genomeSize) {
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
	mutationFlag = true;
	genomeSize += segment.size();
}

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	std::vector<size_t> removeSites;
	size_t removeIndex = index + segmentSize - 1;
	size_t offset = 0;
	//if the mutation is within the genome size
	if (removeIndex <= genomeSize - 1) {
		//check if the site exists in the changelog
		if (changelog.count(index)) {
			//figure out what type of mutation the site is
			//site contains an overwrite mutation
			if (changelog[index].insertOffset == 0 && changelog[index].removeOffset == 0) {
				//std::cout << "inside overwrite mutation " << std::endl;
				changelog[index].removeOffset = segmentSize;
				changelog[index].insertOffset = 0;
				changelog[index].value = (std::byte)0;
			}
			//site contains an insert mutation
			else if (changelog[index].insertOffset > 0) {
				offset += changelog[index].insertOffset;
				std::map<size_t, ChangelogStruct>::iterator itr = changelog.find(index);
				++itr;
				changelog.erase(index);
				if ((itr != changelog.end()) && (std::next(itr) != changelog.end()))
				{
					keys.clear();
					for (std::map<size_t, ChangelogStruct>::iterator it = itr; it != changelog.end(); ++it) {
						keys.push_back(it->first);
					}
					for (auto it = keys.begin(); it != keys.end(); ++it) {
						auto keyIndex = *it;
						auto nh = changelog.extract(keyIndex);
						keyIndex = keyIndex - (size_t)1;
						nh.key() = keyIndex;
						changelog.insert(move(nh));
					}
				}
			}
			//site contains a removal mutation
			else if (changelog[index].removeOffset > 0) {
				//std::cout << "inside removal mutation " << std::endl;
				changelog[index].removeOffset += 1;
			}
		}
		//else the site does not exist, add it to the changelog
		else {
			//std::cout << "inside else case" << std::endl;
			ChangelogStruct c = ChangelogStruct();
			c.removeOffset = segmentSize;
			c.insertOffset = 0;
			c.value = (std::byte)0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		}

		for (size_t i = index; i <= removeIndex; i++) {
			removeSites.push_back(i);
		}
		removeSites.erase(removeSites.begin());
		if (!removeSites.empty()) {
			//loop through sites that will be removed
			for (size_t i = index + 1; i <= removeIndex; i++) {
				//collapse neighboring remove sites
				if (changelog.count(index + 1) && (changelog[index + 1].removeOffset > 0 && changelog[index].removeOffset > 0)) {
					changelog[index].removeOffset += changelog[index + 1].removeOffset;
					changelog.erase(index + 1);
					if (removeSites.size() == 1) {
						removeSites.pop_back();
					}
					else {
						std::vector<size_t>::iterator itr = std::find(removeSites.begin(), removeSites.end(), index + 1);
						removeSites.erase(itr);
						for (int i = 0; i < removeSites.size(); i++) {
							auto remainingSite = removeSites[i];
							auto nh = changelog.extract(remainingSite);
							nh.key() = remainingSite - 1;
							changelog.insert(move(nh));
						}
					}
				}
				//site exists in the changelog
				else if (changelog.count(index) && changelog[index].insertOffset > 0) {
					offset += changelog[i].insertOffset;
					std::map<size_t, ChangelogStruct>::iterator itr = changelog.find(index);
					++itr;
					changelog.erase(index);
					keys.clear();
					for (std::map<size_t, ChangelogStruct>::iterator it = itr; it != changelog.end(); ++it) {
						keys.push_back(it->first);
					}
					for (auto it = keys.begin(); it != keys.end(); ++it) {
						auto keyIndex = *it;
						auto nh = changelog.extract(keyIndex);
						keyIndex = keyIndex - (size_t)1;
						nh.key() = keyIndex;
						changelog.insert(move(nh));
					}
				}
				else if (changelog.count(i)) {
					offset += changelog[i].insertOffset;
					changelog.erase(i);
				}
			}
		}
		if ((changelog.count(index) && offset != 0) && changelog[index].removeOffset > 0) {
			changelog[index].removeOffset -= offset;
		}
		else if (!changelog.count(index) && offset != 0) {
			ChangelogStruct c = ChangelogStruct();
			c.removeOffset = offset;
			c.insertOffset = 0;
			c.value = (std::byte)0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		}
	}
	mutationFlag = true;
	genomeSize -= segmentSize;
}



void StephanieGenome::show() {
	for (int index = 0; index < genomeSize; index++) {
		std::byte& num = GN::genomeRead<std::byte>(this, index);
		std::cout << (int)num << " ";
	}
	std::cout << std::endl;
}

std::vector<std::byte> StephanieGenome::generateNewGenome() {
	bool flag = false;
	size_t index = 0;
	int diff = 0;
	std::vector<std::byte> newSites(genomeSize);
	for (int i = 0; i < genomeSize; i++) {
		//if site does not exist in the changelog
		if (!changelog.count(index)) {;
		//std::cout << "\tindex " << index << " is does not exist in the changelog" << std::endl;
		//std::cout << "\t\tdiff " << diff << std::endl;
		//std::cout << "\tindex " << index << " diff " << diff << " value " << (int)sites[index - diff] << std::endl;
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
			//std::cout << "\tsite " << index << " is an insert mutation" << std::endl;
			diff += changelog[index].insertOffset;
			//std::cout << "\t\tdiff " << diff << std::endl;
			//std::cout << "\tindex " << index << " diff " << diff << " value " << (int)changelog[index].value << std::endl;
			//todo need to update
			newSites[i] = changelog[index].value;
			index++;
		}
		//if the site contains an remove mutation
		else if (changelog.count(index) && changelog[index].removeOffset > 0) {
			//std::cout << "\tsite " << index << " is a remove mutation" << std::endl;
			size_t offset = changelog[index].removeOffset;
			//loop through sites 
			for (size_t j = index; j < index + changelog[index].removeOffset; j++) {
				if (changelog.count(j) && changelog[j].insertOffset > 0) {
					//std::cout << "\t" << j << " exists and is an insert mutation" << std::endl;
					offset -= changelog[j].insertOffset;
				}
			}
			index += offset;
			//std::cout << "\t\tmodified index " << index << std::endl;
			if (i == 0) {
				//std::cout << "\tinside i == 0 case" << std::endl;
				newSites[i] = sites[index];
				if (changelog.count(index)) {
					i += 1;
					newSites[i] = changelog[index].value;
					index++;
				}
			}
			else if (!changelog.count(index)) {
				//std::cout << "\t\t" << index << " not in changelog" << std::endl;
				//std::cout << "\t\tdiff " << diff << std::endl;
				//std::cout << "\t\tsites[" << index << "] " << (int)sites[index] << std::endl;
				if (diff == 0) {
					newSites[i] = sites[index];
				}
				else {
					newSites[i] = sites[index - diff];
				}
			}
			else {
				//std::cout << "\t\t" << index << " in changelog" << std::endl;
				//std::cout << "\t\tdiff " << diff << std::endl;
				//std::cout << "\t\tindex " << index << std::endl; 
				if (changelog[index].removeOffset == 0 && changelog[index].insertOffset == 0) {
					//std::cout << "\t\t\tsite in changelog is an overwrite mutation " << std::endl;
					newSites[i] = changelog[index].value;
				}
				else if (changelog[index].insertOffset > 0) {
					index += offset;
					diff -= 2;
					newSites[i] = sites[index];
					index -= offset;
					index--;
					//std::cout << "\tdiff " << diff << std::endl;
					//std::cout << "\t\t\tindex " << index << std::endl;
				}
			}
			index++;
		}
	}
	for (int index = 0; index < genomeSize; index++) {
		//std::cout << (int)newSites[index] << " ";
	}
	//std::cout << std::endl;
	return newSites;
}

void StephanieGenome::shiftChangelogLeft() {

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

/*std::vector<std::byte> StephanieGenome::generateNewGenome() {
	bool flag = false;
	size_t index = 0;
	size_t diff = 0;
	std::cout << "genomeSize " << genomeSize << std::endl;
	std::vector<std::byte> newSites(genomeSize);
	for (int i = 0; i < genomeSize; i++) {
		std::cout << "i " << i << std::endl;
		std::cout << "index " << index << std::endl;
		std::cout << "diff " << diff << std::endl;
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
			size_t oldIndex = index;
			size_t offset = 0;
			bool flag = false;
			for (size_t i = index; i < (index + changelog[index].removeOffset); i++) {
				if (changelog.count(i)) {
					std::cout << "\t" << i << " exists in changelog" << std::endl;
					if (changelog[i].removeOffset > 0) {
						std::cout << "\t" << i << " contains a removeOffset of " << changelog[i].removeOffset << std::endl;
						offset += changelog[i].removeOffset;
						flag = true;
					}
					else if (changelog[i].insertOffset > 0) {
						std::cout << "\t" << i << " contains an insertOffset of " << changelog[i].insertOffset << std::endl;
						offset -= changelog[i].insertOffset > 0;
						flag = true;
					}
				}
			}

			std::cout << "offset " << offset << std::endl;
			index += offset;
			std::cout << "index " << index << std::endl;
			if (i == 0) {
				std::cout << "inside i==0 case" << std::endl;
				newSites[i] = sites[index];
				if (changelog.count(index)) {
					i += 1;
					newSites[i] = changelog[index].value;
					index++;
				}
			}
			else if (!changelog.count(index) && flag == true) {
				std::cout << "inside " << index << " is not in the changelog and the flag==true" << std::endl;
				newSites[i] = sites[index];
			}
			else {
				std::cout << "inside else case" << std::endl;
				newSites[i] = changelog[index].value;
			}
			index++;
		}
	}
	printChangelog();
	for (int index = 0; index < genomeSize; index++) {
		std::cout << (int)newSites[index] << " ";
	}
	std::cout << std::endl;
	return newSites;
}*/

//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}