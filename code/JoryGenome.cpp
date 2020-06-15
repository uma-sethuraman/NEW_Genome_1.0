#include "JoryGenome.h"
#include <iostream>

JoryGenome::JoryGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* JoryGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void JoryGenome::resize(size_t new_size) {
	size_ = new_size;
};
//GeneView JoryGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
