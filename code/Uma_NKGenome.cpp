#include "Uma_NKGenome.h"
#include <iostream>
#include <random>
#include "Uma_NKEvaluator.h"

Uma_NKGenome::Uma_NKGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* Uma_NKGenome::data() {
	return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void Uma_NKGenome::resize(size_t new_size) {
	size_ = new_size;
};

// Point mutation only right now
void Uma_NKGenome::mutate() {
	int random_index = getRandomInt(0, size_);
	int random_value = getRandomInt(0,1);
	GN::genomeWrite<uint8_t>(this, random_index, random_value);
}

//GeneView Uma_NKGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
