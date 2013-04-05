#ifndef MFL_LINALG_PROJECTION_INCLUDED
#define MFL_LINALG_PROJECTION_INCLUDED

#include "MFL_LinAlg_Matrix.hpp"
#define MFL_CONSTANTS
#include "MFL_math.hpp"
#include <numeric>

namespace MFL{
namespace Graphics3D{

	namespace Projection{
		template <typename T>
		LinAlg::Matrix<T,4> Perspective(T left,T right,T bottom,T top,T Near,T Far){
			using namespace LinAlg;
			Matrix<T,4> outmat;

			T fmn = Far-Near,rml = right - left,tmb = top-bottom;

			outmat(0,0) = 2*Near/rml;
			outmat(1,1) = 2*Near/tmb;
			outmat(0,2) = (right+left)/rml;
			outmat(1,2) = (top+bottom)/tmb;
			outmat(2,2) = -(Far+Near)/fmn;
			outmat(3,2) = -1;
			outmat(2,3) = -2*Far*Near/fmn;
			outmat(3,3) = 0;

			return outmat;
		}

		template <typename T>
		LinAlg::Matrix<T,4> Perspective(T FoV,T aspect,T Near,T Far){
			using namespace LinAlg;
			Matrix<T,4> outmat;
			T cot = static_cast<T>(1/tan(FoV *MFL_PI/360.));
			T fmn = Far-Near;

			outmat(0,0) = cot/aspect;
			outmat(1,1) = cot;
			outmat(2,2) = -(Far+Near)/fmn;
			outmat(2,3) = -2*Far*Near/fmn;
			outmat(3,2) = -1;
			outmat(3,3) = 0;

			return outmat;
		}

		template <typename T>
		LinAlg::Matrix<T,4> Orthogonal(T left,T right,T bottom,T top,T Near,T Far){
			using namespace LinAlg;
			Matrix<T,4> outmat;

			T fmn = Far-Near,rml = right - left,tmb = top-bottom;

			outmat(0,0) = 2/rml;
			outmat(1,1) = 2/tmb;
			outmat(2,2) = -2/fmn;

			outmat(0,3) = -(right+left)/rml;
			outmat(1,3) = -(top+bottom)/tmb;
			outmat(2,3) = -(Far+Near)/fmn;

			return outmat;
		}
	}

	template <typename T,int N>
	LinAlg::Matrix<T,N+1> TranslationMatrix(const LinAlg::Vector<T,N>& vtrans){
		using namespace LinAlg;
		Matrix<T,N+1> outmat;
		for(int i=0; i<N; ++i)
			outmat(i,N) = vtrans(i);
		return outmat;
	}
}
}
#endif//MFL_LINALG_PROJECTION_INCLUDED
