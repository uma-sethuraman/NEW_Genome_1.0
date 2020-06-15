#pragma once

#include <ostream> // operator<<
#include <cstring> // memset

// convenience output for understanding std::byte
std::ostream& operator<<(std::ostream& os, std::byte& byte) {
    os << uint16_t(byte);
    return os;
}

// convenience for writing array-like things (vectors, arrays, etc.)
template <class Iterable, typename T = typename Iterable::value_type>
std::ostream& operator<<(std::ostream& os, Iterable& iterable) {
    os << "[";
    for (int i(-iterable.size()); i<-1; i++)
        os << end(iterable)[i] << ",";
    os << end(iterable)[-1] << "]";
    return os;
}

// convenience for filling a thing with zeros
// useful to zero-out a struct instance before filling it
// and using it to write to the genome,
// BECAUSE: compilers may put arbitrary non-empty data
// inbetween struct fields due to packing boundaries.
template <class T>
void clean(T& structure) {
    memset(&structure, 0, sizeof(T));
}
