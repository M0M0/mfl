#ifndef MFL_RANDOM_INCLUDED
#define MFL_RANDOM_INCLUDED

#include "MFL_Common.hpp"

MFL_BEGIN

template <typename T>
T random(){
	return static_cast<T>(rand())/static_cast<T>(RAND_MAX);
}
#ifdef MFL_LINALG_VECTOR_INCLUDED
template <typename T,int D>
inline LinAlg::Vector<T,D> randomVector(){
	LinAlg::Vector<T,D> vec;
	for(int i=0;i<D;++i)
		vec(i) = random<T>();

	return vec;
}
#endif

#ifdef MFL_LINALG_MATRIX_INCLUDED
template <typename T,int N,int M>
inline LinAlg::Matrix<T,N,M> randomMatrix(){
	LinAlg::Matrix<T,N,M> mat;
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
			mat(i,j) = random<T>();

	return mat;
}
#endif

MFL_END


#endif//MFL_RANDOM_INCLUDED