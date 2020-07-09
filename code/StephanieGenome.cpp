#include "StephanieGenome.h"
#include <iostream>
#include <typeinfo>
#include <stdlib.h>

StephanieGenome::StephanieGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* StephanieGenome::data(size_t index, size_t byteSize) {
	return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
}

void StephanieGenome::resize(size_t new_size) {
	size_ = new_size;
};

void StephanieGenome::insert(size_t index, std::vector<std::byte> segment) {
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
}

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	if (changelog.count(index)) {
		std::cout << "remove: key exists" << std::endl;
		std::cout << "\tindex " << index << " segmentSize " << segmentSize << std::endl << std::endl;

		std::map<size_t, ChangelogStruct>::iterator site = changelog.find(index);
		keys.clear();
		bool deleteSiteFlag = false;
		int deleteSiteIndex = 0;
		int deleteSiteOffset = -99;
		std::cout << "\tkey vector  ";
		for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
			keys.push_back(it->first);
			if (it->second.removeOffset > 0) {
				deleteSiteIndex = it->first;
				deleteSiteOffset = it->second.removeOffset;
				deleteSiteFlag = true;

			}
			std::cout << it->first << " ";
		}
		std::cout << std::endl << std::endl;
		if (!deleteSiteFlag) {
			for (int i = 0; i < segmentSize; i++) {
				std::cout << "\t\n\tloop deleting from changelog " << index << " deleting from keys " << *keys.begin() << std::endl;
				changelog.erase(index);
				keys.erase(keys.begin());
				std::cout << "\t new key vector begin pointer " << *keys.begin() << std::endl;
				for (auto it = keys.begin(); it != keys.end(); ++it) {
					std::cout << "\t key vector  " << *it << std::endl;
					auto keyIndex = *it;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex - (size_t)1;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
				keys.clear();
				site = changelog.find(index);
				std::cout << "\t(" << index << ") sites added to key vector  ";
				for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
					keys.push_back(it->first);
					std::cout << it->first << " ";
				}
			}
		}
		else {
			auto siteIndex = index;
			auto segment = segmentSize;
			for (int i = 0; i < segmentSize; i++) {
				std::cout << "\tdeleteSiteIndex " << deleteSiteIndex << " deleteSiteOffset " << deleteSiteOffset << std::endl;
				if (changelog[*keys.begin()].removeOffset != deleteSiteOffset) {
					std::cout << "\t\tkey " << siteIndex << " != " << deleteSiteIndex << std::endl;
					std::cout << "\t\terasing key " << siteIndex << "..." << std::endl;
					changelog.erase(siteIndex);
					keys.erase(keys.begin());
					segment--;
					std::cout << "\t\tupdating segment value " << segment << "..." << std::endl;
					for (auto it = keys.begin(); it != keys.end(); ++it) {
						auto keyIndex = *it;
						auto nh = changelog.extract(keyIndex);
						keyIndex = keyIndex - (size_t)1;
						nh.key() = keyIndex;
						changelog.insert(move(nh));
					}
					keys.clear();
					site = changelog.find(siteIndex);
					std::cout << "\t\tupdating key vector ";
					for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
						keys.push_back(it->first);
						if (it->second.removeOffset > 0) {
							deleteSiteIndex = it->first;
							deleteSiteOffset = it->second.removeOffset;
						}
						std::cout << it->first << " ";
					}
					std::cout << std::endl << std::endl;
				}
				else {
					std::cout << "\t\tkey " << siteIndex << " == " << deleteSiteIndex << std::endl;
					auto siteOffset = changelog[*keys.begin()].removeOffset;
					changelog[*keys.begin()].removeOffset = siteOffset - 1;
					keys.clear();
					site = changelog.find(siteIndex);
					auto nextIndex = std::next(site, 1);
					siteIndex = nextIndex->first;
					std::cout << "\t\tmove index to next site " << siteIndex << std::endl;
					segment = segment + deleteSiteOffset;
					std::cout << "\t\tupdating segment value " << segment << "..." << std::endl;
					site = changelog.find(siteIndex);
					std::cout << "\t\tupdating key vector ";
					for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
						keys.push_back(it->first);
						if (it->second.removeOffset > 0) {
							deleteSiteIndex = it->first;
							deleteSiteOffset = it->second.removeOffset;
						}
						std::cout << it->first << " ";
					}
					std::cout << std::endl << std::endl;
				}
			}
			changelog[index].removeOffset = segment;
		}

	}
	else {
		std::cout<< "\nremove: key does not exist" << std::endl;
		std::cout<< "\tindex " << index << " segmentSize " << segmentSize << std::endl;
		ChangelogStruct c = ChangelogStruct();
		c.removeOffset = segmentSize;
		c.insertOffset = 0;
		c.value = (std::byte)0;
		changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
		auto affectedSite = index + 1;
		for (int i = 0; i < segmentSize - 1; i++) {
			//std::cout << "\ti(" << i << ") affected site: " << affectedSite << std::endl;
			if (changelog.count(affectedSite) && changelog[affectedSite].removeOffset > 0) {
				//std::cout<< "\t" << affectedSite << " site affected exists in map and has deletion node of " << changelog[affectedSite].removeOffset << std::endl;
				changelog[index].removeOffset = changelog[index].removeOffset + changelog[affectedSite].removeOffset;
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
				changelog[index].removeOffset = changelog[index].removeOffset - 1;
				affectedSite--;
			}
			affectedSite++;
		}
	}

	for (auto site : changelog) {
		auto nextSite = site.first + site.second.removeOffset;
		std::cout << "\tsite: " << site.first << " removeOffset: " << site.second.removeOffset << " nextSite: " << nextSite << std::endl;
		if ((changelog.count(nextSite) && changelog[nextSite].removeOffset > 0) && !changelog.count(site.first + 1)) {
			std::cout << "\tnextSite exists, nextSite removeOffset is > 0, " << site.first+1 << " does not exist in changelog" << std::endl;
			std::cout << "\t\tsite (" << nextSite << ") has deletion node of " << changelog[nextSite].removeOffset << std::endl;
			changelog[site.first].removeOffset = changelog[site.first].removeOffset + changelog[nextSite].removeOffset;
			changelog.erase(nextSite);
		}
		else if ((changelog.count(nextSite) && changelog[nextSite].insertOffset > 0) && (site.second.removeOffset > 0  && !changelog.count(site.first + 1))) {
			std::cout << "\tnextSite exists, nextSite insertOffset is > 0, " << site.first + 1 << " does not exist in changelog" << std::endl;
			std::cout << "\t\tsite: " << site.first << " site+1: " << site.first + 1 << std::endl;
			std::cout << "\t\tnextSite: " << nextSite << " nextSiteInsertOffset: " << changelog[nextSite].insertOffset << std::endl;
			auto currentSiteIndex = site.first;
			ChangelogStruct currentSite = ChangelogStruct();
			currentSite.insertOffset = site.second.insertOffset;
			currentSite.removeOffset = site.second.removeOffset;
			currentSite.value = site.second.value;
			changelog.erase(currentSiteIndex);
			auto nh = changelog.extract(nextSite);
			nh.key() = currentSiteIndex;
			changelog.insert(move(nh));
			auto nextSiteIndex = currentSiteIndex + 1;
			changelog.insert(std::pair<size_t, ChangelogStruct>(nextSiteIndex, currentSite));
			std::cout << "\t\tperformed swap" << std::endl;
		}
	}
}

void StephanieGenome::overwrite(size_t index, std::vector<std::byte> segment) {
	auto siteIndex = index;
	for (auto seg : segment) {
		std::cout << (int)seg << std::endl;
		if (changelog.count(siteIndex)) {
			//std::cout<< "overwrite: key exists" << std::endl;
			//std::cout<< "\siteIndex " << siteIndex << " value " << (int)seg << std::endl;
			changelog[siteIndex].value = seg;
		}
		else {
			//std::cout<< "overwrite: key does not exist" << std::endl;
			//std::cout<< "\siteIndex " << siteIndex << " value " << (int)seg << std::endl;
			ChangelogStruct c = ChangelogStruct();
			c.value = seg;
			c.insertOffset = 0;
			c.removeOffset = 0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(siteIndex, c));
		}
		siteIndex++;
	}
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
	std::cout << "--- end of map ---" << std::endl;
}

void StephanieGenome::generateGenome(AbstractGenome* genome) {
	size_t insertOffset = 0;
	size_t removeOffset = 0;
	for (auto site : changelog) {
		removeOffset = site.second.removeOffset + removeOffset;
		insertOffset = site.second.insertOffset + insertOffset;
	}
	auto updatedGenomeSize = (genome->size() - removeOffset) + insertOffset;
	std::vector<std::byte> modifiedSites;
	modifiedSites.resize(updatedGenomeSize);

	size_t genomeIndex = 0;
	size_t modifiedIndex = 0;
	for (int i = 0; i < modifiedSites.size(); i++) {
		std::cout << std::endl;
		if (!changelog.count(i)) { //if genomeIndex is not on map
			//std::cout << i << " is not on map" <<  std::endl;
			//std::cout<< "i: " << i << std::endl;
			//std::cout<< "genome index: " << genomeIndex << std::endl;
			//std::cout<< "modified index: " << modifiedIndex << std::endl;
			//std::cout<< "value: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, genomeIndex);
			genomeIndex++;
			modifiedIndex++;
		}
		else if ( changelog.count(i) && (changelog[i].removeOffset == 0 && changelog[i].insertOffset == 0) ) { //point mutation
			//std::cout<< i << " is a point mutation" << std::endl;
			//std::cout<< "i: " << i << std::endl;
			//std::cout<< "genome index: " << genomeIndex << std::endl;
			//std::cout<< "modified index: " << modifiedIndex << std::endl;
			//std::cout<< "value: " << (int)changelog[genomeIndex].value << std::endl;
			modifiedSites[i] = changelog[modifiedIndex].value;
			genomeIndex++;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].insertOffset > 0) { //insert mutation
			//std::cout<< i << " is a insert mutation" << std::endl;
			//std::cout<< "i: " << i << std::endl;
			//std::cout<< "genome index: " << genomeIndex << std::endl;
			//std::cout<< "modified index: " << modifiedIndex << std::endl;
			//std::cout<< "value: " << (int)changelog[modifiedIndex].value << std::endl;
			modifiedSites[i] = changelog[modifiedIndex].value;
			modifiedIndex++;
		}
		else if (changelog.count(i) && changelog[i].removeOffset > 0) { //delete mutation
			//std::cout<< i << " is a delete mutation" << std::endl;
			//std::cout<< "\ti: " << i << std::endl;
			////std::cout<< "\toffset: " << abs(changelog[i].removeOffset) << std::endl;
			genomeIndex = genomeIndex + changelog[i].removeOffset;
			////std::cout<< "\tgenome index: " << genomeIndex << std::endl;
			////std::cout<< "\tmodified index: " << modifiedIndex << std::endl;
			////std::cout<< "\tvalue: " << (int)GN::genomeRead<std::byte>(genome, genomeIndex) << std::endl;
			modifiedSites[i] = GN::genomeRead<std::byte>(genome, genomeIndex);
			genomeIndex++;
			modifiedIndex++;
		}
	}
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