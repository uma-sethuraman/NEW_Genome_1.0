#pragma once

#include <ostream> // operator<<
#include <cstring> // memset
#include <valarray>

// convenience output for understanding std::byte
std::ostream& operator<<(std::ostream& os, std::byte& byte) {
    os << uint16_t(byte);
    return os;
}
template <class T>
void printCSV(std::ostream& os, const T& arr){
  os << '[';
  for(int i(0); i<arr.size()-1; ++i)
    os << arr[i] << ',';
  os << arr[arr.size()-1] << ']';
}
template<class T>
auto operator<<( std::ostream& os, const std::valarray<T>& arr ) -> std::ostream& {
  printCSV(os, arr);
  return os;
}
template<class T,std::size_t N>
auto operator<<( std::ostream& os, const std::array<T, N>& arr ) -> std::ostream& {
  printCSV(os, arr);
  return os;
}
template<class T, class A>
auto operator<<( std::ostream& os, const std::vector<T, A>& arr ) -> std::ostream& {
  printCSV(os, arr);
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
