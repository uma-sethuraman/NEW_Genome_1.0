#include "TestGenome.h"
#include <iostream>

TestGenome::TestGenome(size_t _size) {
	sites.resize(_size);
}

std::byte* TestGenome::data(size_t index, size_t byteSize) {
	return static_cast<std::byte*>(&sites[index]); // cast is for demonstration
}

size_t TestGenome::size() {
	return sites.size();
}

void TestGenome::resize(size_t new_size) {
	sites.resize(new_size);
};
