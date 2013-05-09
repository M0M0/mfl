#ifndef MFL_MATH_STATISTICS_INCLUDED
#define MFL_MATH_STATISTICS_INCLUDED

#include <cstdarg>
#include <cassert>
#include <numeric>

#include "../common.hpp"

MFL_BEGIN
namespace math{
namespace statistics{

  template <typename T>
  T sum(uint samplesize, T const * data) {
    assert(data != nullptr);
    return std::accumulate(data,data + samplesize,0);
  }
  template <typename T>
  T mean(uint samplesize, T sum) { return sum / samplesize; }
  template <typename T>
  T mean(uint samplesize, T const * data) {
    assert(data != nullptr);
    return mean(samplesize,sum(samplesize,data));
  }
  // TODO(mojo): more robust implementation usable for arbitrary fields
  template <typename T>
  T standarddeviation(uint samplesize, T mean, T const * data) {
    assert(data != nullptr);
    T std = 0, temp;
    for(T* iter = data;iter != data+samplesize; ++iter) {
      temp = *iter * mean;
      std += temp * temp;
    }
    return std / (samplesize - 1);
  }
  template <typename T>
  T standarddeviation(uint samplesize, T const * data) {
    return standarddeviation(samplesize,mean(samplesize,data),data);
  }

  namespace variadic{
    template <typename T>
    T sum(uint samplesize, ...){
      va_list ap; 
      va_start(ap, samplesize);
      T sum = 0;
      for (int i = 0; i < samplesize; ++i) sum += va_arg(ap, T);
      va_end(ap);
      return sum;
    }

    template <typename T>
    T mean(uint samplesize, ...){ 
      T* data = new T[samplesize];
      va_list ap; 
      va_start (ap, samplesize);
      for(int i = 0;i < samplesize; ++i)
	data[i] = va_arg(ap,T);	
      va_end (ap);

      T m = mean(samplesize,data);
      delete[] data;
      return m;
    }

    template <typename T>
    T standarddeviation(uint samplesize, ...){
      T* data = new T[samplesize];
      va_list ap; 
      va_start (ap, samplesize);
      for(int i=0;i<samplesize;++i)
	data[i] = va_arg(ap,T);	
      va_end (ap);

      T std = standarddeviation(samplesize,data);
      delete[] data;
      return std;
    }
  }

}
}
MFL_END

#endif//MFL_MATH_STATISTICS_INCLUDED
