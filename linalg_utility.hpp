#ifndef MFL_LINALG_UTILITY_INCLUDED
#define MFL_LINALG_UTILITY_INCLUDED

#include "MFL_LinAlg_Common.hpp"

#ifdef MFL_LINALG_VECTOR_INCLUDED
#include <cstdarg>
#include <memory>
#endif

LINALG_BEGIN

#ifdef MFL_LINALG_VECTOR_INCLUDED
template <typename T>
Vector<T,3> TriangleNormal(Vector<T,3> v1,Vector<T,3> v2,Vector<T,3> v3)
	{ return CrossProduct(v2-v1,v3-v1); }


template<typename T>
Vector<T,2> Vector2(const T x,const T y){
	T tmp[] = {x,y};
	return VectorFromArray<T,2>(tmp);
}
template<typename T>
Vector<T,3> Vector3(const T x,const T y,const T z){
	T tmp[] = {x,y,z};
	return VectorFromArray<T,3>(tmp);
}
template<typename T>
Vector<T,4> Vector4(const T x,const T y,const T z,const T w){
	T tmp[] = {x,y,z,w};
	return VectorFromArray<T,4>(tmp);
}

template <typename T,uint D>
const Vector<T,D> VectorVA(T x,...){
	std::unique_ptr<T> ptr(new T[D]);
	va_list ap;
	va_start (ap,x);
	for(uint i=0; i<D; ++i)
		ptr[i] = va_arg(ap,T);
	va_end(ap);
	return VectorFromArray<T,D>(ptr);
}
#endif//MFL_LINALG_VECTOR_INCLUDED

#ifdef MFL_LINALG_MATRIX_INCLUDED
template<typename T>
Matrix<T,3> Scale3(T factor,T x,T y,T z)
	{ return Diag(factor*Vector3<T>(x,y,z)); }
template<typename T>
Matrix<T,4> Scale4(T factor,T x,T y,T z)
	{ return Diag(factor*Vector4<T>(x,y,z,1)); }
template<typename T>
Matrix<T,4> Scale4(T factor,T x,T y,T z,T w)
	{ return Diag(factor*Vector4<T>(x,y,z,w)); }
#endif//MFL_LINALG_VECTOR_INCLUDED

LINALG_END

#endif//MFL_LINALG_UTILITY_INCLUDED