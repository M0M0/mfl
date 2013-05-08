// TODO: add functionality to check if unit quaternion
#ifndef MFL_MATH_QUATERNION_INCLUDED
#define MFL_MATH_QUATERNION_INCLUDED

#include "MFL_LinAlg_Vector.hpp"
#include "MFL_LinAlg_Utility.hpp"

namespace MFL{
namespace math{

	template <typename T>
	class Quaternion{
	private:
		T _s;
		LinAlg::Vector<T,3> _v;

	public:

		Quaternion(T s=0,T x=0,T y=0,T z=0):_s(s),_v(0){
			_v(0) = x;
			_v(1) = y;
			_v(2) = z;
		}
		Quaternion(T s,LinAlg::Vector<T,3> v):_s(s),_v(v){}
		Quaternion(const Quaternion& q):_s(q._s),_v(q._v){}

		Quaternion& inverse(){
			// should be magnitude(?)
			T tmp = 1/magnitudeSq();
			_s *= tmp;
			_v *= tmp;
			conjugate();
			return *this;
		}
		const T magnitudeSq(){
			return _s*_s + _v*_v;
		}
		Quaternion& conjugate(){
			_v = -_v;
			return *this;
		}

		Quaternion& operator+=(const Quaternion& rhs){
			_s += rhs._s;
			_v += rhs._v;
			return *this;
		}
		Quaternion& operator-=(const Quaternion& rhs){
			_s -= rhs._s;
			_v -= rhs._v;
			return *this;
		}
		
		Quaternion& operator*=(const Quaternion& rhs){
			Quaternion tmp(*this);
			_s = tmp._s*rhs._s - tmp._v*rhs._v;
			_v = tmp._s*rhs._v + tmp._v*rhs._s - CrossProduct(tmp._v,rhs._v);
			return *this;
		}
		Quaternion& operator*=(const T			 rhs){
			_r *= rhs;
			_v *= rhs;
			return *this;
		}

		const Quaternion operator+(const Quaternion& rhs) const{
			return Quaternion(*this)+=rhs;
		}
		const Quaternion operator-(const Quaternion& rhs) const{
			return Quaternion(*this)-=rhs;
		}

		const Quaternion operator*(const Quaternion& rhs) const{
			return Quaternion(*this)*=rhs;
		}
		const Quaternion operator*(const T			 rhs) const{
			return Quaternion(*this)*=rhs;
		}
#ifdef MFL_LINALG_MATRIX_INCLUDED
		const LinAlg::Matrix<T,3> GetRotationMatrix3() const {
			LinAlg::Matrix<T,3> outmat;
			T N = magnitudeSq();

			if(N>0){
				T w = 2/N;
				T x=_v(0)*w,y=_v(1)*w,z=_v(2)*w;

				T sx=_s*x,   sy=_s*y,   sz=_s*z,
				  xx=_v(0)*x,xy=_v(0)*y,xz=_v(0)*z,
							 yy=_v(1)*y,yz=_v(1)*z,
										zz=_v(2)*z;
#define SC1 static_cast<T>(1.0)
				T tmp[] = {SC1-(yy+zz),		  xy-sz,		   xz+sy,
								 xy+sz, SC1-(xx+zz),		   yz-sx,
								 xz-sy,		  yz+sx,     SC1-(xx+yy)};
				outmat = *(reinterpret_cast<LinAlg::Matrix<T,3>*>(tmp));
#undef SC1
			}
			return outmat;
		}

		const LinAlg::Matrix<T,4> GetRotationMatrix4() const{
			return LinAlg::Homogenize(GetRotationMatrix3());
		}
#endif//MFL_LINALG_MATRIX_INCLUDED
#ifdef _OSTREAM_
		template <typename Q>
		friend std::ostream& operator<<(std::ostream&,const Quaternion<Q>&);
#endif//_OSTREAM_
	};


#ifdef _OSTREAM_
	template <typename Q>
	inline std::ostream& operator<<(std::ostream& out,const Quaternion<Q>& p){
		out << p._s << '\t';
		out << p._v;
		return out;
	}
#endif

	template<typename T>
	Quaternion<T> RotationQuaternion(T angle,T x,T y,T z){
		T Angle_Half = angle/2;
		LinAlg::Vector<T,3> vec = LinAlg::Vector3<T>(x,y,z);
		vec.normalise();
		vec*=sin(Angle_Half);
		return Quaternion<T>(cos(Angle_Half), vec);
	}
	// might be useless
	template<typename T>
	Quaternion<T> PositionQuaternion(T x,T y,T z){
		return Quaternion<T>(0,x,y,z);
	}
	template<typename T>
	Quaternion<T> PositionQuaternion(LinAlg::Vector<T,3> v){
		return Quaternion<T>(0,v);
	}


}
}

#endif//MFL_MATH_QUATERNION_INCLUDED