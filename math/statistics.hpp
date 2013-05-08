// TODO: switch to unsigned int for samplesize!
// Discussion : Use std::runtime_exception instead of assert?

#ifndef MFL_MATH_STATISTICS_INCLUDED
#define MFL_MATH_STATISTICS_INCLUDED

#include <cstdarg>
#include <cassert>

namespace MFL{
namespace math{
namespace statistics{

	template <typename T>
	T sum(int samplesize, T* data){
		assert(data!=nullptr);
		T sum = 0;
		for (T* iter=data;iter!=data+samplesize; ++iter)
			sum += *iter;
		return sum;
	}

	template <typename T>
	T mean(int samplesize,T sum)
	{ return sum/samplesize; }

	template <typename T>
	T mean(int samplesize, T* data){
		assert(data!=nullptr);
		return mean(samplesize,sum(samplesize,data));
	}

	template <typename T>
	T standarddeviation(int samplesize, T mean, T* data){
		assert(data!=nullptr);
		T std = 0; //switch from operator* to something more robust/useable on vectors
		for(T* iter=data;iter!=data+samplesize;++iter) // or define output in another way
			std += ((*iter)*mean)*((*iter)*mean);	
		return std/(samplesize-1);
	}
	template <typename T>
	T standarddeviation(int samplesize, T* data)
	{ return standarddeviation(samplesize,mean(samplesize,data),data); }


	namespace variadic{
		template <typename T>
		T sum(int samplesize, ...){
			va_list ap; 
			va_start (ap, samplesize);
			T sum = 0;
			for (int i = 0; i < samplesize; ++i)
				sum += va_arg (ap, T);
			va_end (ap);
			return sum;
		}

		template <typename T>
		T mean(int samplesize, ...){ 
			T* data = new T[samplesize];
			va_list ap; 
			va_start (ap, samplesize);
			for(int i=0;i<samplesize;++i)
				data[i] = va_arg(ap,T);	
			va_end (ap);

			T m = mean(samplesize,data);
			delete[] data;
			return m;
		}

		template <typename T>
		T standarddeviation(int samplesize, ...){
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
}
#endif//MFL_MATH_STATISTICS_INCLUDED