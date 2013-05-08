#ifndef MFL_MATH_COMPLEX_INCLUDED
#define MFL_MATH_COMPLEX_INCLUDED

namespace MFL{
namespace math{
	template <typename T>
	class Complex{
	private:
		T _re;
		T _im;

	public:
		Complex():_re(0),_im(0){}
		Complex(T re_i, T im_i=0):_re(re_i),_im(im_i){}

		Complex& operator+=(const Complex& rhs){
			_re += rhs._re;
			_im += rhs._im;
		}
		const Complex& operator+(const Complex& rhs) const{
			return Complex(_re,_im).operator+=(rhs);
		}

		Complex& operator-=(const Complex& rhs){
		}
	};


}
}

#endif//MFL_MATH_COMPLEX_INCLUDED