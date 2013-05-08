#ifndef MFL_MATH_METAFUNCTIONS_INCLUDED
#define MFL_MATH_METAFUNCTIONS_INCLUDED

namespace MFL{
namespace math{
namespace metafunctions{
	namespace detail{
		// request removal!
		template <int b,int c>
		struct mult{
			enum { value = c*b };
		};
		//EpsilonTensor calculation
		template <int i,int j,int k>
		struct epsilontensor{
			enum { value = mult<mult<i-j,j-k>::value,k-i>::value /2 };
		};

		template <int n> struct factorial{
			enum { value=n*factorial<n-1>::value };
		};
		template <> struct factorial<0>{
			enum { value=1 };
		};

	}

	template <int n>
	inline int factorial()
	{ return detail::factorial<n>::value; }

	template <int i,int j,int k>
	inline int EpsilonTensor()
	{ return detail::epsilontensor<i,j,k>::value; }

}
}
}

#endif//MFL_MATH_METAFUNCTIONS_INCLUDED