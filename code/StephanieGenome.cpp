#include "StephanieGenome.h"
#include <iostream>
#include <typeinfo>
#include <stdlib.h>

//TODO: implement random access - aka can you get to any point in the genome
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
		std::cout << "insert: key does not exist" << std::endl;
		std::cout << "\tindex " << index << " value " << (int)value << std::endl;
		keys.clear();
		
		if (changelog.count(index)) {
			std::cout << " is present " << std::endl;
			std::map<size_t, ChangelogStruct>::iterator site = changelog.begin();
			for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
				std::cout << "\t" << it->first << ", " << (int)it->second.value << " " << it->second.offset << std::endl;
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
			ChangelogStruct c = ChangelogStruct();
			c.value = value;
			c.offset = 1;
			changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		}
	}
}

void StephanieGenome::deleteMutation(size_t index, int siteOffset) {
	if (changelog.count(index)) {
		//std::cout << "delete: key exists" << std::endl;
		//std::cout << "\tindex " << index << " siteOffset " << siteOffset << std::endl;

		std::map<size_t, ChangelogStruct>::iterator site = changelog.find(index);
		keys.clear();
		bool deleteSiteFlag = false;
		int deleteSiteIndex = 0;
		int deleteSiteOffset = -99;
		for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
			keys.push_back(it->first);
			if (it->second.offset < 0) {
				deleteSiteIndex = it->first;
				deleteSiteOffset = it->second.offset;
				deleteSiteFlag = true;

			}
			//std::cout << it->first << " ";
		}
		//std::cout << std::endl;
		if (!deleteSiteFlag) {
			for (int i = 0; i < abs(siteOffset); i++) {
				////std::cout << "\t loop deleting from changelog " << index << " deleting from keys " << *keys.begin() << std::endl;
				changelog.erase(index);
				keys.erase(keys.begin());
				////std::cout << "\t new key vector begin pointer " << *keys.begin() << std::endl;
				for (auto it = keys.begin(); it != keys.end(); ++it) {
					////std::cout << "\t key vector  " << *it << std::endl;
					auto keyIndex = *it;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex - (size_t)1;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
				keys.clear();
				site = changelog.find(index);
				////std::cout << "\t(" << index << ") sites added to key vector  ";
				for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
					keys.push_back(it->first);
					////std::cout << it->first << " ";
				}
			}
		}
		else {
			auto siteIndex = index;
			for (int i = 0; i < abs(siteOffset); i++) {
				if (changelog[*keys.begin()].offset != deleteSiteOffset) {
					//std::cout << "\tchangelog.offset: " << changelog[*keys.begin()].offset << " deleteSiteOffset: " << deleteSiteOffset << std::endl;
					//std::cout << "\tindex: " << siteIndex << std::endl;
					changelog.erase(siteIndex);
					keys.erase(keys.begin());
					for (auto it = keys.begin(); it != keys.end(); ++it) {
						auto keyIndex = *it;
						auto nh = changelog.extract(keyIndex);
						keyIndex = keyIndex - (size_t)1;
						nh.key() = keyIndex;
						changelog.insert(move(nh));
					}
					keys.clear();
					site = changelog.find(siteIndex);
					//std::cout << "\t(" << index << ") sites added to key vector  ";
					for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
						keys.push_back(it->first);
						if (it->second.offset < 0) {
							deleteSiteIndex = it->first;
							deleteSiteOffset = it->second.offset;
						}
						//std::cout << it->first << " ";
					}
				}
				else {
					//std::cout << "\tchangelog.offset: " << changelog[*keys.begin()].offset << " deleteSiteOffset: " << deleteSiteOffset << std::endl;
					//std::cout << "\tindex: " << siteIndex << std::endl;
					auto siteOffset = changelog[*keys.begin()].offset;
					changelog[*keys.begin()].offset = siteOffset - 1;
					keys.clear();
					site = changelog.find(siteIndex);
					auto nextIndex = std::next(site, 1);
					siteIndex = nextIndex->first;
					//std::cout << "\tnew index: " << siteIndex << std::endl;
					site = changelog.find(siteIndex);
					//std::cout << "\t(" << index << ") sites added to key vector  ";
					for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
						keys.push_back(it->first);
						if (it->second.offset < 0) {
							deleteSiteIndex = it->first;
							deleteSiteOffset = it->second.offset;
						}
						//std::cout << it->first << " ";
					}
				}
			}
		}
	}
	else {
		//std::cout<< "\ndelete: key does not exist" << std::endl;
		//std::cout<< "\tindex " << index << " siteOffset " << siteOffset << std::endl;
		ChangelogStruct c = ChangelogStruct();
		c.offset = siteOffset;
		c.value = (std::byte)0;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		auto affectedSite = index + 1;
		for (int i = 1; i < abs(siteOffset); i++) {
			//std::cout<< "\t affected site: " << affectedSite << std::endl;
			if (changelog.count(affectedSite) && changelog[affectedSite].offset < 0) {
				//std::cout<< "\t" << affectedSite << " site affected exists in map and has deletion node of " << changelog[affectedSite].offset << std::endl;
				changelog[index].offset = changelog[index].offset + changelog[affectedSite].offset;
				changelog.erase(affectedSite);
			}
			else if (changelog.count(affectedSite)) {
				//std::cout<< "\t" << affectedSite << " site affected exists in map" << std::endl;
				std::map<size_t, ChangelogStruct>::iterator site = changelog.find(affectedSite);
				keys.clear();
				for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
					keys.push_back(it->first);
					//std::cout<< "\t site added to key vector " << it->first << std::endl;
				}
				//std::cout<< "\t deleting site " << affectedSite << std::endl;
				changelog.erase(affectedSite);
				keys.erase(keys.begin());
				for (auto it = keys.begin(); it != keys.end(); ++it) {
					auto keyIndex = *it;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex - (size_t)1;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
				changelog[index].offset = changelog[index].offset + 1;
				affectedSite--;
			}
			affectedSite++;
		}
	}

	for (auto site : changelog) {
		auto affectedSite = site.first + 1;
		for (int i = 0; i < abs(site.second.offset) - 1; i++) {
			if (changelog.count(affectedSite) && changelog[affectedSite].offset < 0) {
				//std::cout<< "deleted site " << affectedSite << " has deletion node of " << changelog[affectedSite].offset << std::endl;
				//std::cout<< "\t(" << index << ") " << "site offset " << changelog[affectedSite].offset << std::endl;
				changelog[index].offset = changelog[index].offset + changelog[affectedSite].offset;
				changelog.erase(affectedSite);
			}
			affectedSite++;
		}

		auto checkNextSite = site.first + abs(site.second.offset);
		if ((changelog.count(checkNextSite) && site.second.offset < 0) && !changelog.count(site.first + 1)) {
			//std::cout<< "\tsite " << site.first << " site offset " << site.second.offset << std::endl;
			//std::cout<< "\t\tsite " << checkNextSite << " site offset " << changelog[checkNextSite].offset << std::endl;
			auto currentSiteIndex = site.first;
			ChangelogStruct currentSite = ChangelogStruct();
			currentSite.offset = site.second.offset;
			currentSite.value = site.second.value;
			changelog.erase(site.first);
			auto keyIndex = checkNextSite;
			auto nh = changelog.extract(keyIndex);
			keyIndex = currentSiteIndex;
			nh.key() = keyIndex;
			changelog.insert(move(nh));
			auto nextSiteIndex = currentSiteIndex + 1;
			changelog.insert(std::pair<size_t, ChangelogStruct>(nextSiteIndex, currentSite));
		}
	}
}

void StephanieGenome::pointMutation(size_t index, std::byte value) {
	if(changelog.count(index)) {
		//std::cout<< "point: key exists" << std::endl;
		//std::cout<< "\tindex " << index << " value " << (int)value << std::endl;
		changelog[index].value = value;
	}
	else {
		//std::cout<< "point: key does not exist" << std::endl;
		//std::cout<< "\tindex " << index << " value " << (int)value << std::endl;
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
	std::cout << "--- end of map ---" << std::endl;
}

void StephanieGenome::generateGenome(AbstractGenome* genome) {
	int sum = 0;
	for (auto site : changelog) {
		auto offset = site.second.offset;
		sum = sum + offset;
	}
	sum = genome->size() + sum;
	std::vector<std::byte> modifiedSites;
	modifiedSites.resize(sum);

	size_t genomeIndex = 0;
	size_t modifiedIndex = 0;
	for (int i = 0; i < modifiedSites.size(); i++) {
		//std::cout << std::endl;
		if (!changelog.count(i)) { //if genomeIndex is not on map
			//std::cout<< i << " is not on map" <<  std::endl;
			//std::cout<< "i: " << i << std::endl;
			//std::cout<< "genome index: " << genomeIndex << std::endl;
			//std::cout<< "modified index: " << modifiedIndex << std::endl;
			//std::cout<< "value: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, genomeIndex);
			genomeIndex++;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].offset == 0) { //point mutation
			//std::cout<< i << " is a point mutation" << std::endl;
			//std::cout<< "i: " << i << std::endl;
			//std::cout<< "genome index: " << genomeIndex << std::endl;
			//std::cout<< "modified index: " << modifiedIndex << std::endl;
			//std::cout<< "value: " << (int)changelog[genomeIndex].value << std::endl;
			modifiedSites[i] = changelog[modifiedIndex].value;
			genomeIndex++;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].offset > 0) { //insert mutation
			//std::cout<< i << " is a insert mutation" << std::endl;
			//std::cout<< "i: " << i << std::endl;
			//std::cout<< "genome index: " << genomeIndex << std::endl;
			//std::cout<< "modified index: " << modifiedIndex << std::endl;
			//std::cout<< "value: " << (int)changelog[modifiedIndex].value << std::endl;
			modifiedSites[i] = changelog[modifiedIndex].value;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].offset < 0) { //delete mutation
			//std::cout<< i << " is a delete mutation" << std::endl;
			//std::cout<< "\ti: " << i << std::endl;
			//std::cout<< "\toffset: " << abs(changelog[i].offset) << std::endl;
			genomeIndex = genomeIndex + abs(changelog[i].offset);
			//std::cout<< "\tgenome index: " << genomeIndex << std::endl;
			//std::cout<< "\tmodified index: " << modifiedIndex << std::endl;
			//std::cout<< "\tvalue: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, genomeIndex);
			genomeIndex++;
			modifiedIndex++;
		}
	}
	//std::cout << std::endl;
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