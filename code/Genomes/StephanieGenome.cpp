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
	// Mutations have not occured to the parent genome
	// Return parent genome
	if (!mutationFlag) {
		return static_cast<std::byte*>(&sites[index]);
	}
	// Offspring genome has been generated
	// Return offspring genome
	else if (genomeGenerated == true) {
		return static_cast<std::byte*>(&modifiedSites[index]);
	}
	// Offspring genome has not been generated
	// Return offspring genome
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
	// Create a clone genome
	// Generate offspring genome
	// Assign offspring genome to clone genome
	// Return clone genome
	if (forceCopy) {
		AbstractGenome* genomeClone = new StephanieGenome(genomeSize);
		generateNewGenome();
		(static_cast<StephanieGenome*>(genomeClone))->sites = modifiedSites;
		return genomeClone;
	}
	// Return parent genome
	else {
		return new StephanieGenome(*this);
	}
}

void StephanieGenome::overwrite(size_t index, const std::vector<std::byte>& segment) {
	if (index <= genomeSize - 1) {
		bool offsetFlag = false;
		size_t lastIndex = segment.size() - 1;
		size_t removeOffset = 0;
		size_t site = index;
		std::vector<size_t> sitesAffected;

		// Loop through segment vector
		for (auto seg : segment) {

			// Index exists in changelog
			// Site in changelog is not a remove mutation
			if (changelog.count(site) && changelog[site].removeOffset == 0) {
				changelog[site].value = seg;
			}

			// Index exists in changelog
			// Site in changelog is a remove mutation
			else if (changelog.count(site) && changelog[site].removeOffset > 0) {

				// Get a vector of sites in the changelog that will be affected by the remove mutation
				for (size_t i = site + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						sitesAffected.push_back(i);
					}
				}

				// Shift the affected sites in the changelog to the right by 1
				shiftChangelog(false, sitesAffected, 1);

				// Index exists in changelog
				// Get removeOffset value from site in changelog
				// Erase site
				if (changelog.count(site)) {
					offsetFlag = true;
					removeOffset = changelog[site].removeOffset;
					changelog.erase(site);
				}
				// Create site in changelog with overwrite mutation
				addToChangelog(site, 0, 0, seg);
			}

			// Index does not exist in changelog
			// Create site in changelog with overwrite mutation
			else {
				addToChangelog(site, 0, 0, seg);
			}

			site++;
		}

		// A site affected by the overwrite mutation was a remove mutation
		// Create site in changelog with remove mutation
		if (offsetFlag == true) {
			addToChangelog(site, 0, removeOffset, (std::byte)0);
		}
	}
	mutationFlag = true;
	genomeGenerated = false;
}

void StephanieGenome::insert(size_t index, const std::vector<std::byte>& segment) {
	if (index <= genomeSize) {
		bool insertFlag = false;
		std::vector<size_t> sitesAffected;
		
		// Get an iterator at first site affected
		std::map<size_t, ChangelogStruct>::iterator site;

		// Index exists in changelog
		// Get the iterator of the site in the changelog
		if (changelog.count(index)) {
			site = changelog.find(index);
			insertFlag = true;
		}
		// Index does not exist in changelog
		// Get the iterator of the next site in the changelog
		else {
			for (auto it = changelog.rbegin(); it != changelog.rend(); ++it) {
				if (it->first > index) {
					site = changelog.find(it->first);
					insertFlag = true;
				}
			}
		}

		// If changelog is not empty and a site after the index site exists
		if (!changelog.empty() && insertFlag == true) {
			// Get a vector of sites in the changelog that will be affected by the insert mutation
			for (std::map<size_t, ChangelogStruct>::iterator it = site; it != changelog.end(); ++it) {
				sitesAffected.push_back(it->first);
			}

			// Shift the affected sites in the changelog to the right by segment vector size
			shiftChangelog(false, sitesAffected, segment.size());
		}

		// Create site in changelog with insert mutation
		size_t indexCounter = index;
		for (size_t i = 0; i < segment.size(); i++) {
			addToChangelog(indexCounter, 1, 0, segment[i]);

			indexCounter++;
		}
		genomeSize += segment.size();
	}
	mutationFlag = true;
	genomeGenerated = false;
}

void StephanieGenome::remove(size_t index, size_t segmentSize) {
	if ((index + segmentSize - 1) <= genomeSize - 1) {
		std::vector<size_t> sitesAffected;

		// Index exists in the changelog
		if (changelog.count(index)) {
			bool removeFlag = false;
			bool insertFlag = false;

			// Get a vector of sites in the changelog that will be affected by the remove mutation
			for (size_t i = index; i < (index + segmentSize); i++) {
				if (changelog.count(i)) {
					if (changelog[i].removeOffset > 0) {
						removeFlag = true;
					}
					else if (changelog[i].insertOffset > 0) {
						insertFlag = true;
					}
					sitesAffected.push_back(i);
				}
			}

			// All sites affected are overwrite mutations
			if (removeFlag == false && insertFlag == false) {
				int sitesRemoved = 0;

				// Erase sites affected
				for (auto siteAffected : sitesAffected) {
					changelog.erase(siteAffected);
					sitesRemoved--;
				}

				// Get a new vector of the rest of the sites in the changelog
				sitesAffected.clear();
				for (size_t i = index + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						sitesAffected.push_back(i);
					}
				}

				// Shift the sites in the changelog to the left by segmentSize
				shiftChangelog(true, sitesAffected, segmentSize);

				size_t indexChangelog = index;
				// If segmentSize sites were removed from the changelog
				// Insert site with remove mutation at the end of the changelog
				if (abs(sitesRemoved) == segmentSize && segmentSize > 1) {
					indexChangelog = sitesAffected[sitesAffected.size() - 1];
					indexChangelog = indexChangelog - segmentSize + 1;
				}

				// Create site in changelog with remove mutation
				addToChangelog(indexChangelog, 0, segmentSize, (std::byte)0);
			}

			// There is at least one site with an insert mutation affected
			else if (removeFlag == false && insertFlag == true) {
				size_t modifiedRemoveOffset = segmentSize;

				// Erase sites affected
				for (auto siteAffected : sitesAffected) {
					modifiedRemoveOffset--;
					changelog.erase(siteAffected);
				}

				// Get a new vector of the rest of the sites in the changelog
				sitesAffected.clear();
				for (size_t i = index + 1; i < genomeSize; i++) {
					if (changelog.count(i)) {
						sitesAffected.push_back(i);
					}
				}

				// Shift the sites in the changelog to the left by modifiedRemoveOffset
				shiftChangelog(true, sitesAffected, segmentSize);

				// If the number sites removed from the changelog was not segmentSize
				// Site at index does not exist in the changelog
				if (modifiedRemoveOffset != 0 && !changelog.count(index)) {
					addToChangelog(index, 0, modifiedRemoveOffset, (std::byte)0);

				}
			}

			// Index site in changelog is an insert mutation
			// Next site in changelog is a remove mutation
			// Collapse sites into one remove mutation
			else if ((changelog[index].insertOffset > 0) && changelog.count(index + 1) && changelog[index + 1].removeOffset > 0) {
				size_t modifiedRemoveOffset = (segmentSize - changelog[index].insertOffset) + changelog[index + 1].removeOffset;
				changelog.erase(index + 1);
				changelog[index].removeOffset = modifiedRemoveOffset;
				changelog[index].insertOffset = 0;
				changelog[index].value = (std::byte)0;

			}
		}

		// Index does not exist in the changelog
		else {
			// Create site in changelog with remove mutation
			addToChangelog(index, 0, segmentSize, (std::byte)0);

			// Get a vector of sites in the changelog that will be affected by the remove mutation
			for (size_t i = index + 1; i < (index + segmentSize); i++) {
				if (changelog.count(i)) {
					sitesAffected.push_back(i);
				}
			}

			// Erase affected sites from the changelog if site is an overwrite or insert mutation
			// Calculate insert mutation sites affected
			size_t insertSitesAffected = 0;
			for (auto siteAffected : sitesAffected) {
				if (changelog[siteAffected].insertOffset > 0) 
					insertSitesAffected += changelog[siteAffected].insertOffset;
				if (changelog[siteAffected].removeOffset == 0)
					changelog.erase(siteAffected);
			}

			// Update remove offset at index by insert sites removed
			changelog[index].removeOffset = segmentSize - insertSitesAffected;

			// Get a new vector of the rest of the sites in the changelog
			sitesAffected.clear();
			for (size_t i = index + 1; i < genomeSize; i++) {
				if (changelog.count(i) && changelog[i].removeOffset == 0) {
					sitesAffected.push_back(i);
				}
			}

			// Shift the sites in the changelog to the left by modifiedSegmentSize
			for (auto it = sitesAffected.begin(); it != sitesAffected.end(); ++it) {
				auto keyIndex = *it;
				auto nh = changelog.extract(keyIndex);
				keyIndex = keyIndex - changelog[index].removeOffset;
				nh.key() = keyIndex;

				// If the affected site is a remove mutation and has a key collision with site at index
				// Erase site at index
				// Shift newly created remove mutation site in the changelog to the right by 1
				// Create affected site at index
				if (keyIndex == index) {
					size_t removeOffset = changelog[index].removeOffset;
					changelog.erase(index);
					addToChangelog(index + 1, 0, removeOffset, (std::byte)0);
				}
				changelog.insert(move(nh));
			}

			// If the site after index site is a remove mutation
			// Collapse remove mutation sites
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
}

void StephanieGenome::show() {
	for (int index = 0; index < genomeSize; index++) {
		std::byte& num = GN::genomeRead<std::byte>(this, index);
		std::cout << (int)num << " ";
	}
	std::cout << std::endl;
}

// Adds site at index to changelog
void StephanieGenome::addToChangelog(size_t index, size_t insert, size_t remove, std::byte value) {
	ChangelogStruct c = ChangelogStruct();
	c.insertOffset = insert;
	c.removeOffset = remove;
	c.value = value;
	changelog.insert(std::pair<size_t, ChangelogStruct>(index, c));
}

// Generates an offspring genome from the parent genome and changelog
void StephanieGenome::generateNewGenome() {
	modifiedSites.resize(genomeSize);
	int offset = 0;

	// Create an offspring genome
	for (int i = 0; i < genomeSize; i++) {

		// Index does not exist in the changelog
		if (!changelog.count(i)) {
			modifiedSites[i] = sites[i + offset];
		}

		// Index exists in changelog and site is a overwrite mutation
		else if (changelog.count(i) && (changelog[i].removeOffset == 0 && changelog[i].insertOffset == 0)) {
			modifiedSites[i] = changelog[i].value;
		}

		// Index exists in changelog and site is an insert mutation
		else if (changelog.count(i) && changelog[i].insertOffset > 0) {
			modifiedSites[i] = changelog[i].value;
			offset -= 1;
		}

		// Index exists in changelog and site is a remove mutation
		else if (changelog.count(i) && changelog[i].removeOffset > 0) {
			offset += (int)changelog[i].removeOffset;
			modifiedSites[i] = sites[i + offset];
		}
	}

	//print modified genome
	//std::cout << "\nGENERATED GENOME: ";
	//for (int index = 0; index < genomeSize; index++) {
		//std::cout << (int)modifiedSites[index] << " ";
	//}
	//std::cout << std::endl;
}

// Prints changelog
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

// Shifts changelog left or right based on offset and sites affected
void StephanieGenome::shiftChangelog(bool shiftLeft, std::vector<size_t>& sitesAffected, size_t offset) {
	// Shift sites left
	if (shiftLeft) {
		for (auto it = sitesAffected.begin(); it != sitesAffected.end(); ++it) {
			auto keyIndex = *it;
			auto nh = changelog.extract(keyIndex);
			keyIndex = keyIndex - (size_t)offset;
			nh.key() = keyIndex;
			changelog.insert(move(nh));
		}
	} 
	// Shift sites right
	else {
		for (auto rit = sitesAffected.rbegin(); rit != sitesAffected.rend(); ++rit) {
			auto keyIndex = *rit;
			auto nh = changelog.extract(keyIndex);
			keyIndex = keyIndex + (size_t)offset;
			nh.key() = keyIndex;
			changelog.insert(move(nh));
		}
	}
}


//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}