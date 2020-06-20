#include "StephanieGenome.h"
#include <iostream>
#include <typeinfo>

StephanieGenome::StephanieGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
	changelog.resize(_size);
}

std::byte* StephanieGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

std::list<std::byte>::iterator StephanieGenome::changelogDataIterator(){
	return changelog.begin();
}

void StephanieGenome::resize(size_t new_size) {
	size_ = new_size;
};



//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
