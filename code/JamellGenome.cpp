#include "JamellGenome.h"
#include <iostream>

JamellGenome::JamellGenome(size_t _size): AbstractGenome(_size),sites(_size){
        sites.resize(_size);
}

std::byte* JamellGenome::data() {
        return static_cast<std::byte*>(sites.data()); // cast is for demonstration
}

void JamellGenome::resize(size_t new_size) {
        size_ = new_size;
};
//GeneView JamellGenome::geneView() {
//  //GeneView gv;
//  //...
//  //return gv;
//}
