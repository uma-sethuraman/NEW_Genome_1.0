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
		return static_cast<std::byte*>(&sites[index]);
	}
	else if (genomeGenerated == true) {
		return static_cast<std::byte*>(&modifiedSites[index]);
	}
	else {
		generateNewGenome();
		genomeGenerated = true;
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

AbstractGenome* StephanieGenome::clone(bool forceCopy) {
	if (forceCopy) {
		AbstractGenome* genomeClone = new StephanieGenome(genomeSize);
		generateNewGenome();
		(static_cast<StephanieGenome*>(genomeClone))->sites = modifiedSites;
		return genomeClone;
	}
	else {
		return new StephanieGenome(*this);
	}
}


void StephanieGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
	//std::cout << "=========== OVERWRITE " << index << " " << segment.size() << " ============ " << std::endl;

	//if the mutation is within the genome size
	bool offsetFlag = false;
	if (index <= genomeSize - 1) {
		auto site = index;
		size_t removeOffset = 0;
		size_t lastIndex = segment.size() - 1;
		//loop through the segments
		for (auto seg : segment) {
			//if the site exists in the changelog and is not a remove mutation update the value
			if (changelog.count(site) && changelog[site].removeOffset == 0) {
				//std::cout << "site exists in changelog and is not a remove mutation" << std::endl;
				changelog[site].value = seg;
			}

			//else if the site exists in the changelog and is a remove mutation 
			else if (changelog.count(site) && changelog[site].removeOffset > 0) {
				//std::cout << "site exists in changelog site is a remove mutation" << std::endl;
				//get vector of sites affected
				keys.clear();
				//std::cout << "\tvector of sites affected: ";
				for (size_t i = site + 1; i < genomeSize; i++) {
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

				if (!changelog.count(site)) {
					//add in overwrite mutation
					ChangelogStruct c = ChangelogStruct();
					c.value = seg;
					c.insertOffset = 0;
					c.removeOffset = 0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(site, c));
				}
				else {
					offsetFlag = true;
					removeOffset = changelog[site].removeOffset;
					//std::cout << "\toffset " << removeOffset << " added " << std::endl;
					changelog.erase(site);
					ChangelogStruct c = ChangelogStruct();
					c.value = seg;
					c.insertOffset = 0;
					c.removeOffset = 0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(site, c));
				}
			}

			//if the site does not exist in the changelog add in new site
			else {
				//std::cout << "site does not exist in changelog" << std::endl;
				ChangelogStruct c = ChangelogStruct();
				c.value = seg;
				c.insertOffset = 0;
				c.removeOffset = 0;
				changelog.insert(std::pair<size_t, ChangelogStruct>(site, c));
			}

			site++;
		}

		if (offsetFlag == true) {
			ChangelogStruct c = ChangelogStruct();
			c.value = (std::byte)0;
			c.insertOffset = 0;
			c.removeOffset = removeOffset;
			changelog.insert(std::pair<size_t, ChangelogStruct>(site, c));
		}
	}
	mutationFlag = true;
	genomeGenerated = false;
	//printChangelog();
}

void StephanieGenome::insert(size_t index, const std::vector<std::byte>& segment) {
	//std::cout << "=========== INSERT " << index << " " << segment.size() << " ============ " << std::endl;
	//std::cout << "=========== SEGMENTS ";
	bool insertFlag = false;
	for (int i = 0; i < segment.size(); i++) {
		//std::cout << (int)segment[i] << " ";
	}
	//std::cout << " ============ " << std::endl;
	//printChangelog();
	//if the mutation is within the genome size
	if (index <= genomeSize) {
		size_t indexCounter = index;
		//generate a vector of sites that will be shifted to the right
		std::map<size_t, ChangelogStruct>::iterator site;
		if (changelog.count(indexCounter)) {
			//std::cout << indexCounter << " exists in the changelog" << std::endl;
			site = changelog.find(indexCounter);
			insertFlag = true;
		}

		else {
			//std::cout << indexCounter << " does not exist in the changelog" << std::endl;
			if (!changelog.empty()) {
				//std::cout << "\tchangelog is not empty" << std::endl;
			}
			for (auto it = changelog.rbegin(); it != changelog.rend(); ++it) {
				//std::cout << "\tit->first " << it->first << std::endl;
				//std::cout << "\tchangelog size " << changelog.size() << std::endl;
				if (it->first > indexCounter) {
					//std::cout << "\tit->first " << it->first << std::endl;
					site = changelog.find(it->first);
					insertFlag = true;
				}
			}
			//std::cout << "\texiting else case" << std::endl;
		}

		if (!changelog.empty() && insertFlag == true) {
			keys.clear();
			//std::cout << "sites affected: ";
			for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
				//std::cout << it->first << " ";
				keys.push_back(it->first);
			}
			//std::cout << std::endl;


			//std::cout << "shifting sites to the right..." << std::endl;
			//shift sites in vector to the right
			for (auto rit = keys.rbegin(); rit != keys.rend(); ++rit) {
				auto keyIndex = *rit;
				auto nh = changelog.extract(keyIndex);
				keyIndex = keyIndex + (size_t)segment.size();
				nh.key() = keyIndex;
				changelog.insert(move(nh));
			}
		}

		//std::cout << "adding in new sites..." << std::endl;
		for (size_t i = 0; i < segment.size(); i++) {
			//add in new site
			ChangelogStruct c = ChangelogStruct();
			c.value = segment[i];
			c.insertOffset = 1;
			c.removeOffset = 0;
			changelog.insert(std::pair<size_t, ChangelogStruct>(indexCounter, c));
			indexCounter++;
		}
	}
	mutationFlag = true;
	genomeGenerated = false;
	genomeSize += segment.size();
	//printChangelog();
}

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	//std::cout << "============ REMOVE " << index << " " << segmentSize << " ============ " << std::endl;
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
				size_t segmentCheck = segmentSize;
				//loop through sites affected vector
				for (auto siteAffected : keys) {
					//erase the sites in the changelog
					changelog.erase(siteAffected);
					segmentCheck--;
				}

				//get vector of sites affected
				keys.clear();
				////std::cout << "\tvector of sites affected: ";
				for (size_t i = index + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						////std::cout << i << " ";
						keys.push_back(i);
					}
				}
				////std::cout << std::endl;

				//shift sites in vector to the left by segmentSize
				for (auto it = keys.begin(); it != keys.end(); ++it) {
					auto keyIndex = *it;
					auto nh = changelog.extract(keyIndex);
					keyIndex = keyIndex - (size_t)segmentSize;
					nh.key() = keyIndex;
					changelog.insert(move(nh));
				}
				//std::cout << segmentCheck << std::endl;

				if (segmentCheck == 0 && segmentSize > 1) {
					//std::cout << "ooooveerrrr here" << std::endl;
					size_t lastKeyIndex = keys.size() - 1;
					size_t changelogIndex = keys[lastKeyIndex];
					changelogIndex = changelogIndex - segmentSize + 1;
					ChangelogStruct c = ChangelogStruct();
					c.removeOffset = segmentSize;
					c.insertOffset = 0;
					c.value = (std::byte)0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(changelogIndex, c));
				}
				//else if (segmentCheck != 0) {
				else {
					//std::cout << "in here" << std::endl;
					//add in new site with segmentSize removeOffset
					ChangelogStruct c = ChangelogStruct();
					c.removeOffset = segmentSize;
					c.insertOffset = 0;
					c.value = (std::byte)0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
				}
			}

			//sites affected contain insert mutations 
			else if (removeOffset == 0 && insertOffset > 0) {
				//std::cout << "sites affected contain insert mutations" << std::endl;
				//newSegmentSize = segmentSize
				size_t newSegmentSize = segmentSize;
				//loop through sites affected vector
				for (auto siteAffected : keys) {
					//subtract newSegmentSize by the insertOffset for each site 
					newSegmentSize--;
					changelog.erase(siteAffected);
				}
				//std::cout << "newSegmentSize " << newSegmentSize << std::endl;
				//get vector of sites affected
				keys.clear();
				////std::cout << "\tvector of sites affected: ";
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

				if (newSegmentSize != 0 && !changelog.count(index)) {
					ChangelogStruct c = ChangelogStruct();
					c.removeOffset = newSegmentSize;
					c.insertOffset = 0;
					c.value = (std::byte)0;
					changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));

				}
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
				if (changelog[siteAffected].removeOffset == 0) {
					changelog.erase(siteAffected);
				}
			}
			//update the segmentSize of the remove mutation
			changelog[index].removeOffset = segmentSize - modifiedSegmentSize;

			//get vector of sites affected
			keys.clear();
			//std::cout << "\tvector of sites affected: ";
			for (size_t i = index + 1; i < genomeSize; i++) {
				if (changelog.count(i) && changelog[i].removeOffset == 0) {
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
			for (size_t i = index + 1; i <= index + changelog[index].removeOffset; i++) {
				if (changelog.count(i) && changelog[i].removeOffset > 0) {
					changelog[index].removeOffset += changelog[i].removeOffset;
					changelog.erase(i);
				}

			}
		}
		genomeSize -= segmentSize;
	}
	mutationFlag = true;
	genomeGenerated = false;
	//printChangelog();
}

void StephanieGenome::show() {
	//std::cout << "=========== SHOW ============ " << std::endl;
	//printChangelog();
	for (int index = 0; index < genomeSize; index++) {
		std::byte& num = GN::genomeRead<std::byte>(this, index);
		std::cout << (int)num << " ";
	}
	std::cout << std::endl;
}

void StephanieGenome::generateNewGenome() {
	//std::cout << "============ GENERATE NEW GENOME ============" << std::endl;
	//printChangelog();
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
	//std::cout << "\nGENERATED GENOME: ";
	for (int index = 0; index < genomeSize; index++) {
		//std::cout << (int)modifiedSites[index] << " ";
	}
	//std::cout << std::endl;
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

//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}