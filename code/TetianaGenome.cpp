#include "TetianaGenome.h"

TetianaGenome::TetianaGenome(size_t _size): AbstractGenome(_size),sites(_size){
	sites.resize(_size);
}

std::byte* TetianaGenome::data(size_t index, size_t byteSize) {
	return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
}

void TetianaGenome::resize(size_t new_size) {
	size_ = new_size;
}

void TetianaGenome::mutate() {
    
    // point mutation - sets random value (range [0, 2^k)) to random index (range [0, n))
    // TODO rand function
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist1(0, size_);
    
    int random_site = dist1(e2); // range [0, size_), q: should I use sites.size() instead?
    //std::cout << "random_site: " << random_site << std::endl;
    int k = 2; // how to not hard-code it?
    std::uniform_real_distribution<> dist2(0, std::pow(2, k));
    int random_value = dist2(e2);
    
    sites[random_site] = (std::byte)random_value;
    //std::cout << "sites[random_site]: " << std::to_integer<int>(sites[random_site]) << std::endl;
}

// starting at index, write values in segement genome between genome[index-1] and genome[index]
void TetianaGenome::insert(size_t index, std::vector<std::byte> segment) {
    std::cout << "insert has not been written for this genome class" << std::endl;
    exit(1);
}
