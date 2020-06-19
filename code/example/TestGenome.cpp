#include "TestGenome.h"
#include <iostream>

TestGenome::TestGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* TestGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void TestGenome::resize(size_t new_size) {
	size_ = new_size;
};
//GeneView TestGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
