#ifndef MFL_IO_INCLUDED
#define MFL_IO_INCLUDED

#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <stdexcept>

namespace io {

std::string ReadFile(char const * file) {
  std::string fstring;
  std::ifstream fptr(file);
  if (fptr.is_open())
    fstring = std::string((std::istreambuf_iterator<char>(fptr)),
			   std::istreambuf_iterator<char>());
  else
    throw std::runtime_error(
      std::string("The file \'")+file+"\' could not be opened!");
  return fstring;
}

template <typename Iter_t>
void CopyToFile(Iter_t first,Iter_t last,char const * name){
  std::copy(first,last,
    std::ostream_iterator<decltype(*first)>(std::ofstream(name)));
}

bool isanagram(std::string a,std::string b){
  using namespace std;
  transform(a.begin(),a.end(), a.begin(), ::tolower);
  transform(b.begin(),b.end(), b.begin(), ::tolower);
  for (char test = 'a'; test<='z';++test)
    if (count(a.begin(),a.end(),test) != count(b.begin(),b.end(),test))
      return false;
  return true;
}

}

#endif//MFL_IO_INCLUDED
