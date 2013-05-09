#ifndef MFL_MATH_INCLUDED
#define MFL_MATH_INCLUDED

#include <cmath>
#include <cstdarg>

#include "../common.hpp"

MFL_BEGIN

namespace math{

  template <typename T>
  T Sgn(T i) {
    return i >= 0 ? 1 : -1;
  }
  int KroneckerDelta(int i,int j){
    return i == j ? 1 : 0;
  }	
  int LeviCivitaSymbol(int i,int j, int k){
    return ((i - j)*(j - k)*(k - i)) / 2;
  }
  int LeviCivitaSymbol(int i,int j, int k,int l){
    return ((i - j)*(i - k)*(i - l)*(j - k)*(j - l)*(k - l)) / 12;
  }

  int LeviCivitaSymbol_Vargs(int nargs,...){
    va_list args;
    int* permutation = new int[nargs];
    va_start( args, nargs );
    for (int i=0;i<nargs;++i) permutation[i] = va_arg(args,int);
    va_end( args );
    int prod=1;
    for (int i = 0;i < nargs-1; ++i)
      for (int j = i+1;j < nargs; ++j)
	prod * = permutation[j] - permutation[i];
    delete[] permutation;
    return Sgn(prod);
  }

}

MFL_END

#endif //MFL_MATH_INCLUDED

// 21 Digit accuracy
#ifdef MFL_CONSTANTS

#define MFL_SQRT2	1.41421356237309504880
#define MFL_SQRT2_INV	0.70710678118654752440

#define MFL_SQRT3	1.73205080756887729352
#define MFL_SQRT3_INV	0.57735026918962576450

#define MFL_SQRT5       2.23606797749978969640
#define MFL_SQRT5_INV	0.44721359549995793928

#define MFL_PI		3.14159265358979323846 
#define MFL_PI_INV	0.31830988618379067153

#define MFL_E		2.71828182845904523536
#define MFL_E_INV	0.36787944117144232159

#endif//MFL_CONSTANTS
