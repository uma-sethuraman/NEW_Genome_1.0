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
		generateNewGenome();
		return static_cast<std::byte*>(&modifiedSites[index]);
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
	std::cout << "=========== OVERWRITE " << index << " " << segment.size() << " ============ " << std::endl;
	//if the mutation is within the genome size
	if (index <= genomeSize - 1) {
		auto site = index;
		//loop through the segments
		for (auto seg : segment) {
			//if the site exists in the changelog and is not a remove mutation update the value
			if (changelog.count(site) && changelog[site].removeOffset == 0) {
				changelog[site].value = seg;
			}

			//else if the site exists in the changelog and is a remove mutation 
			else if (changelog.count(site) && changelog[site].removeOffset > 0) {
				std::cout << "in here!" << std::endl;
				//get vector of sites affected
				keys.clear();
				//std::cout << "\tvector of sites affected: ";
				for (size_t i = index + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						//std::cout << i << " ";
						keys.push_back(i);
					}
				}
				//std::cout << std::endl;

				//shift sites in vector to the right
				for (auto rit = keys.rbegin(); rit != keys.rend(); ++rit) {
					auto keyIndex = *rit;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex + (size_t)1;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}

				//add in overwrite mutation
				ChangelogStruct c = ChangelogStruct();
				c.value = seg;
				c.insertOffset = 0;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(site, c));


			}

			//if the site does not exist in the changelog add in new site
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
	printChangelog();
}

void StephanieGenome::insert(size_t index, const std::vector<std::byte>& segment) {
	std::cout << "=========== INSERT " << index << " " << segment.size() << " ============ " << std::endl;
	//if the mutation is within the genome size
	if (index <= genomeSize) {
		size_t indexCounter = index;
		//loop through the sites that will be inserted
		for (int i = 0; i < segment.size(); i++) {
			//check if the site exists in the changelog
			if (changelog.count(indexCounter)) {
				//generate a vector of sites that will be shifted to the right
				keys.clear();
				std::map<size_t, ChangelogStruct>::iterator site = changelog.find(indexCounter);
				for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
					keys.push_back(it->first);
				}

				//shift sites in vector to the right
				for (auto rit = keys.rbegin(); rit != keys.rend(); ++rit) {
					auto keyIndex = *rit;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex + (size_t)1;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}

				//add in new site
				ChangelogStruct c = ChangelogStruct();
				c.value = segment[i];
				c.insertOffset = 1;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
			}
			//site does not exist in the changelog
			else {
				//add in new site
				ChangelogStruct c = ChangelogStruct();
				c.value = segment[i];
				c.insertOffset = 1;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));

				//generate a vector of sites that will be shifted to the right
				keys.clear();
				std::map<size_t, ChangelogStruct>::iterator site = changelog.find(indexCounter);
				for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
					keys.push_back(it->first);
				}
				keys.erase(keys.begin());

				//shift sites in vector to the right
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
	printChangelog();
}

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	std::cout << "============ REMOVE " << index << " " << segmentSize << " ============ " << std::endl;
	//get a vector of sites that may be affected if they exist in the changelog
	//if the mutation is within the genome size
	if ((index + segmentSize - 1) <= genomeSize - 1) {
		//site exists in changelog
		if (changelog.count(index)) {
			//get a vector of sites affected
			size_t removeOffset = 0;
			size_t insertOffset = 0;
			keys.clear();
			//std::cout << "vector of sites affected: ";
			for (size_t i = index; i < (index + segmentSize); i++) {
				if (changelog.count(i)) {
					removeOffset += changelog[i].removeOffset;
					insertOffset += changelog[i].insertOffset;
					//std::cout << i << " ";
					keys.push_back(i);
				}
			}
			//std::cout << std::endl;

			//sites affected are overwrite mutations
			if (removeOffset == 0 && insertOffset == 0) {
				//loop through sites affected vector
				for (auto siteAffected : keys){
					//erase the sites in the changelog
					changelog.erase(siteAffected);
				}

				//add in new site with segmentSize removeOffset
				ChangelogStruct c = ChangelogStruct();
				c.removeOffset = segmentSize;
				c.insertOffset = 0;
				c.value = (std::byte)0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));


				//get vector of sites affected
				keys.clear();
				//std::cout << "\tvector of sites affected: ";
				for (size_t i = index + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						//std::cout << i << " ";
						keys.push_back(i);
					}
				}
				//std::cout << std::endl;

				//shift sites in vector to the left by segmentSize
				for (auto it = keys.begin(); it != keys.end(); ++it) {
					auto keyIndex = *it;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex - (size_t)segmentSize;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
			}

			//sites affected contain insert mutations 
			else if (removeOffset == 0 && insertOffset > 0) {
				//newSegmentSize = segmentSize
				size_t newSegmentSize = segmentSize;
				//loop through sites affected vector
				for (auto siteAffected : keys) {
					//subtract newSegmentSize by the insertOffset for each site 
					changelog.erase(siteAffected);
				}

				//get vector of sites affected
				keys.clear();
				//std::cout << "\tvector of sites affected: ";
				for (size_t i = index + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						//std::cout << i << " ";
						keys.push_back(i);
					}
				}
				//std::cout << std::endl;

				//shift sites in vector to the left by newSegmentSize
				for (auto it = keys.begin(); it != keys.end(); ++it) {
					auto keyIndex = *it;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex - (size_t)segmentSize;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}


				/*
				if (newSegmentSize != 0) {
					std::cout << "\tinside here?" << std::endl;
					//write a site for index with newSegmentSize
					ChangelogStruct c = ChangelogStruct();
					c.removeOffset = newSegmentSize;
					c.insertOffset = 0;
					c.value = (std::byte)0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
				}
				else {
					//shift neighboring inserts over
					//get a vector of the neighboring inserts
					std::cout << "\tnewSegmentSize = " << newSegmentSize << std::endl;
					keys.clear();
					std::cout << "\tsites in vector: ";
					for (size_t i = index + segmentSize; i < genomeSize; i++) {
						if (changelog.count(i)) {
							std::cout << i << " ";
							keys.push_back(i);
						}
					}
					std::cout << std::endl;
					std::cout << "\tshift sites in vector to the left by " << segmentSize << std::endl;
					//shift sites in vector to the left by segmentSize
					for (auto it = keys.begin(); it != keys.end(); ++it) {
						auto keyIndex = *it;
						auto nh = changelog.extract(keyIndex);
						keyIndex = keyIndex - (size_t)segmentSize;
						nh.key() = keyIndex;
						changelog.insert(move(nh));
					}

				}
				*/
			}

			//index site is an insert mutation
			//next site is a remove mutation
			//collapse two sites into one remove mutation
			else if ((changelog[index].insertOffset > 0) && changelog.count(index + 1) && changelog[index + 1].removeOffset > 0) {
				//std::cout << "\tindex site is an insert mutation and the next site is a remove mutation" << std::endl;
				size_t modifiedSegmentSize = (segmentSize - changelog[index].insertOffset) + changelog[index + 1].removeOffset;
				changelog.erase(index + 1);
				changelog[index].removeOffset = modifiedSegmentSize;
				changelog[index].insertOffset = 0;
				changelog[index].value = (std::byte)0;

			}
		}

		//site does not exist in changelog
		else {
			//std::cout << index << " does not exist in changelog" << std::endl;
			ChangelogStruct c = ChangelogStruct();
			c.removeOffset = segmentSize;
			c.insertOffset = 0;
			c.value = (std::byte)0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));

			//get a vector of sites affected
			size_t removeOffset = 0;
			size_t insertOffset = 0;
			keys.clear();
			for (size_t i = index + 1; i < (index + segmentSize); i++) {
				if (changelog.count(i)) {
					removeOffset += changelog[i].removeOffset;
					insertOffset += changelog[i].insertOffset;
					keys.push_back(i);
				}
			}

			//erase the sites in the changelog
			//get modifiedSegmentSize
			size_t modifiedSegmentSize = 0;
			for (auto siteAffected : keys) {
				if (changelog[siteAffected].insertOffset > 0) modifiedSegmentSize += changelog[siteAffected].insertOffset;
				changelog.erase(siteAffected);
			}
			
			//update the segmentSize of the remove mutation
			changelog[index].removeOffset = segmentSize - modifiedSegmentSize;

			//get vector of sites affected
			keys.clear();
			//std::cout << "\tvector of sites affected: ";
			for (size_t i = index + 1; i < genomeSize; i++) {
				if (changelog.count(i)) {
					//std::cout << i << " ";
					keys.push_back(i);
				}
			}
			//std::cout << std::endl;

			//shift sites in vector to the left by modifiedSegmentSize
			size_t offset = changelog[index].removeOffset;
			for (auto it = keys.begin(); it != keys.end(); ++it) {
				auto keyIndex = *it;
				auto nh = changelog.extract(keyIndex);
				keyIndex = keyIndex - offset;
				if (keyIndex == index) {
					//std::cout << "key collision!" << std::endl;
					changelog.erase(index);
					nh.key() = keyIndex;
					changelog.insert(move(nh));

					ChangelogStruct c = ChangelogStruct();
					c.removeOffset = offset;
					c.insertOffset = 0;
					c.value = (std::byte)0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(index + 1, c));
				}
				else {
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
			}

		}
		genomeSize -= segmentSize;
	}
	mutationFlag = true;
	printChangelog();
}

/*void StephanieGenome::remove(size_t index, size_t segmentSize) {
	std::cout << "============ REMOVE " << index << " " << segmentSize << " ============ " << std::endl;
	std::vector<size_t> removeSites;
	size_t removeIndex = index + segmentSize - 1;
	size_t offset = 0;
	//if the mutation is within the genome size
	if (removeIndex <= genomeSize - 1) {
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
				changelog[index].removeOffset += 1;
			}
		}

		//else the site does not exist, add it to the changelog
		else {
			ChangelogStruct c = ChangelogStruct();
			c.removeOffset = segmentSize;
			c.insertOffset = 0;
			c.value = (std::byte)0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		}

		//generate a vector of sites that will be removed
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
					offset = segmentSize - offset;
					offset += changelog[i].removeOffset;
					changelog.erase(i);
				}
			}
		}
		if ((changelog.count(index) && offset != 0) && changelog[index].removeOffset > 0) {
			changelog[index].removeOffset -= offset;
		}
		else if ((changelog.count(index) && offset != 0)) {
			changelog[index].removeOffset = offset;
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
}*/


void StephanieGenome::show() {
	for (int index = 0; index < genomeSize; index++) {
		std::byte& num = GN::genomeRead<std::byte>(this, index);
		std::cout << (int)num << " ";
	}
	std::cout << std::endl;
}

void StephanieGenome::generateNewGenome() {
	std::cout << "============ GENERATE NEW GENOME ============" << std::endl;
	printChangelog();
	modifiedSites.resize(genomeSize);
	int offset = 0;
	size_t index = 0;

	//recreate the new genome
	for (int i = 0; i < genomeSize; i++) {
		//std::cout<< "i = " << i << std::endl;
		//std::cout<< "index = " << index << std::endl;
		//std::cout<< "offset = " << offset << std::endl;

		//if site does not exist in the changelog
		if (!changelog.count(index)) {
			//std::cout<< "\t== no mutation ==" << std::endl;
			modifiedSites[i] = sites[index + offset];
		}

		//if the site contains an overwrite mutation
		else if (changelog.count(index) && (changelog[index].removeOffset == 0 && changelog[index].insertOffset == 0)) {
			//std::cout<< "\t== overwrite mutation ==" << std::endl;
			modifiedSites[i] = changelog[index].value;
		}

		//if the site contains an insert mutation
		else if (changelog.count(index) && changelog[index].insertOffset > 0) {
			//std::cout<< "\t== insert mutation ==" << std::endl;
			modifiedSites[i] = changelog[index].value;
			offset -= 1;
		}

		//if the site contains a remove mutation
		else if (changelog.count(index) && changelog[index].removeOffset > 0) {
			//std::cout<< "\t== remove mutation ==" << std::endl;
			offset += changelog[index].removeOffset;
			//std::cout<< "\toffset " << offset << std::endl;
			modifiedSites[i] = sites[index + offset];
		}
		index++;
	}

	//print modified genome
	std::cout << "\nGENERATED GENOME: ";
	for (int index = 0; index < genomeSize; index++) {
		std::cout << (int)modifiedSites[index] << " ";
	}
	std::cout << std::endl;
}

/*
//new signature version
void StephanieGenome::generateNewGenome() {
	std::cout << "============ GENERATE NEW GENOME ============" << std::endl;
	printChangelog();
	size_t originalGenome = 0;
	size_t modifiedGenome = 0;
	modifiedSites.resize(genomeSize);

	//recreate the new genome
	for (int i = 0; i < genomeSize; i++) {
		std::cout << "i = " << i << std::endl;
		std::cout << "originalGenome = " << originalGenome << std::endl;
		std::cout << "modifiedGenome = " << modifiedGenome << std::endl;

		//if site does not exist in the changelog
		if (!changelog.count(modifiedGenome)) {
			std::cout << "\t== no mutation ==" << std::endl;
			std::cout << "\t" << (int)sites[originalGenome] << std::endl;
			modifiedSites[i] = sites[originalGenome];
			originalGenome++;
			modifiedGenome++;
		}

		//if the site contains an overwrite mutation
		else if (changelog.count(modifiedGenome) && (changelog[modifiedGenome].removeOffset == 0 && changelog[modifiedGenome].insertOffset == 0)) {
			std::cout << "\t== overwrite mutation ==" << std::endl;
			std::cout << "\t" << (int)changelog[modifiedGenome].value << std::endl;
			modifiedSites[i] = changelog[modifiedGenome].value;
			originalGenome++;
			modifiedGenome++;
		}

		//if the site contains an insert mutation
		else if (changelog.count(modifiedGenome) && changelog[modifiedGenome].insertOffset > 0) {
			std::cout << "\t== insert mutation ==" << std::endl;
			std::cout << "\t" << (int)changelog[modifiedGenome].value << std::endl;
			modifiedSites[i] = changelog[modifiedGenome].value;
			modifiedGenome++;
		}

		//if the site contains a remove mutation
		else if (changelog.count(modifiedGenome) && changelog[modifiedGenome].removeOffset > 0) {
			std::cout << "\t== remove mutation ==" << std::endl;
			originalGenome += changelog[modifiedGenome].removeOffset;
			std::cout << "\tupdated originalGenome = " << originalGenome << std::endl;

			//if site removeOffset = 1 and site remove index is 0
			if (modifiedGenome == 0 && changelog[modifiedGenome].removeOffset == 1) {
				modifiedSites[0] = sites[originalGenome];
				originalGenome++;
				modifiedGenome++;
			}


			//else if site removeOffset = 1 and the updated genome index exists in the changelog
			else if (changelog.count(originalGenome) && changelog[modifiedGenome].removeOffset == 1) {
				std::cout << "\tsite has a removeOffset = 1 and " << originalGenome << " is in changelog" << std::endl;
				std::cout << "\t" << (int)changelog[originalGenome].value << std::endl;
				modifiedSites[i] = changelog[originalGenome].value;
				originalGenome++;
				modifiedGenome++;
			}

			//else if site removeOffset = 1 and the updated genome index does not in the changelog
			else if (!changelog.count(originalGenome) && changelog[modifiedGenome].removeOffset == 1) {
				std::cout << "\tsite has a removeOffset = 1 and " << originalGenome << " is not in changelog" << std::endl;
				std::cout << "\t" << (int)sites[originalGenome] << std::endl;
				modifiedSites[i] = sites[originalGenome];
				modifiedGenome = originalGenome;
				originalGenome++;
				modifiedGenome++;
			}

			//the site has a removeOffset > 1
			else if (changelog[modifiedGenome].removeOffset > 1) {
				std::cout << "\tsite has a removeOffset > 1 " << std::endl;
				std::cout << "\t" << (int)sites[originalGenome] << std::endl;
				modifiedSites[i] = sites[originalGenome];
				originalGenome++;
				modifiedGenome++;
			}
		}
	}

	//print modified genome
	std::cout << "\nGENERATED GENOME: ";
	for (int index = 0; index < genomeSize; index++) {
		std::cout << (int)modifiedSites[index] << " ";
	}
	std::cout << std::endl;
}*/


/*
//old signature version
void StephanieGenome::generateNewGenome() {
	printChangelog();
	bool flag = false;
	size_t index = 0;
	int diff = 0;
	modifiedSites.resize(genomeSize);
	//recreate the new genome
	for (int i = 0; i < genomeSize; i++) {
		//if site does not exist in the changelog
		if (!changelog.count(index)) {
			;
			std::cout << "\t" << index << " does not exist in the changelog" << std::endl;
			std::cout << "\t\tdiff " << diff << std::endl;
			std::cout << "\t\tindex " << index << " diff " << diff << " value " << (int)sites[index - diff] << std::endl;
			modifiedSites[i] = sites[index - diff];
			index++;
		}

		//if the site contains an overwrite mutation
		else if (changelog.count(index) && (changelog[index].removeOffset == 0 && changelog[index].insertOffset == 0)) {
			modifiedSites[i] = changelog[index].value;
			index++;
		}

		//if the site contains an insert mutation
		else if (changelog.count(index) && changelog[index].insertOffset > 0) {
			std::cout << "\t" << index << " insert mutation" << std::endl;
			diff += changelog[index].insertOffset;
			std::cout << "\t\tdiff " << diff << std::endl;
			std::cout << "\t\tindex " << index << " diff " << diff << " value " << (int)changelog[index].value << std::endl;
			//todo need to update
			modifiedSites[i] = changelog[index].value;
			index++;
		}

		//if the site contains a remove mutation
		else if (changelog.count(index) && changelog[index].removeOffset > 0) {
			std::cout << "\t" << index << " remove mutation" << std::endl;
			size_t offset = changelog[index].removeOffset;
			//loop through sites 
			std::cout << "\t\tlooping through sites " << index << " to " << index + changelog[index].removeOffset << std::endl;
			for (size_t j = index; j < index + changelog[index].removeOffset; j++) {
				if (changelog.count(j) && changelog[j].insertOffset > 0) {
					std::cout << "\t\t" << j << " is an insert mutation" << std::endl;
					std::cout << "\t\toffset " << offset << std::endl;
					offset -= changelog[j].insertOffset;
					std::cout << "\t\toffset " << offset << std::endl;
				}
			}
			index += offset;
			std::cout << "\t\tmodified index " << index << std::endl;
			if (i == 0) {
				std::cout << "\t\t\tinside i == 0 case" << std::endl;
				modifiedSites[i] = sites[index];
				if (changelog.count(index)) {
					i += 1;
					modifiedSites[i] = changelog[index].value;
					index++;
				}
			}
			else if (!changelog.count(index)) {
				std::cout << "\t\t\t" << index << " not in changelog" << std::endl;
				std::cout << "\t\t\tdiff " << diff << std::endl;
				std::cout << "\t\t\tsites[" << index << "] " << (int)sites[index] << std::endl;
				if (diff == 0) {
					modifiedSites[i] = sites[index];
				}
				else {
					modifiedSites[i] = sites[index - diff];
				}
			}
			else {
				std::cout << "\t\t\t" << index << " in changelog" << std::endl;
				std::cout << "\t\t\tdiff " << diff << std::endl;
				std::cout << "\t\t\tindex " << index << std::endl; 
				if (changelog[index].removeOffset == 0 && changelog[index].insertOffset == 0) {
					std::cout << "\t\t\t\tsite in changelog is an overwrite mutation " << std::endl;
					modifiedSites[i] = changelog[index].value;
				}
				else if (changelog[index].insertOffset > 0) {
					index += offset;
					diff -= 2;
					std::cout << "\t\t\tindex " << index << " diff " << diff << " value " << (int)sites[index] << std::endl;
					modifiedSites[i] = sites[index];
					index -= offset;
					index--;
					std::cout << "\t\t\tdiff " << diff << std::endl;
					std::cout << "\t\t\tindex " << index << std::endl;
				}
			}
			index++;
		}
	}

	//print modified genome
	std::cout << "\nGENERATED GENOME: ";
	for (int index = 0; index < genomeSize; index++) {
		std::cout << (int)modifiedSites[index] << " ";
	}
	std::cout << std::endl;
}*/

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