#ifndef MFL_AI_PERCEPTRON_INCLUDED
#define MFL_AI_PERCEPTRON_INCLUDED

#include "MFL_LinAlg_Vector.hpp"
#include "MFL_AI_ActivationFunctions.hpp"

namespace MFL{
namespace ai{

	template <typename T,int N,template<typename> class S>
	class Perceptron{
		MFL::LinAlg::Vector<T,N> _w; // weights
		T _b;    // bias
		static AF<T,S> _af;
	public:
		Perceptron(T b_in,T w_in[N])
			:_b(b_in),_w(w_in,true),_af(){}
		Perceptron(T b_in,MFL::LinAlg::Vector<T,N> w_in)
			:_b(b_in),_w(w_in),_af(){}
		T operator() (T x[N]){
			return _af.f(w*(*static_cast<MFL::LinAlg::Vector<T,N>*>(x)) + b);
		}
	};

}
}

#endif//MFL_AI_PERCEPTRON_INCLUDED