#include "StephanieGenome.h"
#include <iostream>

StephanieGenome::StephanieGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* StephanieGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void StephanieGenome::resize(size_t new_size) {
	size_ = new_size;
};
//GeneView StephanieGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
