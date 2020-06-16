#include "TetianaGenome.h"
#include <iostream>

TetianaGenome::TetianaGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* TetianaGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void TetianaGenome::resize(size_t new_size) {
	size_ = new_size;
}

//GeneView TetianaGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
