#ifndef MFL_IO_INCLUDED
#define MFL_IO_INCLUDED

#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace io {

std::string ReadFile(std::string file) {
  std::string fstring;
  std::ifstream fptr(file);
  if (fptr.is_open()) {
    fstring = std::string((std::istreambuf_iterator<char>(fptr)),
			   std::istreambuf_iterator<char>());
  } else {
    throw std::runtime_error("The file \'" + file + "\' could not be opened!");
  }
  return fstring;
}

template <typename Iter_t>
void WriteFile(Iter_t first,Iter_t last,char const * name) {
  std::copy(first,last,
	    std::ostream_iterator<decltype(*first)>(std::ofstream(name)));
}

}

#endif//MFL_IO_INCLUDED
