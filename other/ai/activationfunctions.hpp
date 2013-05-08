#ifndef MFL_AI_ACTIVATIONFUNCTIONS_INCLUDED
#define MFL_AI_ACTIVATIONFUNCTIONS_INCLUDED

#include <math.h>

namespace MFL{
namespace ai{
	//enum Activationfunctions { None, HyperbolicTangent, LogisticSignoid, Threshold };

	template <typename T>
	class Activationfunction{
	public:
		virtual T f(T input);
		const static bool IsAF(){ return true; }
	};

	template <typename T>
	class HyperbolicTangentAF:public Activationfunction<T>{
	public:
		T f(T input){ return tanh(input); }
	};
	template <typename T>
	class LogisticSignoidAF:public Activationfunction<T>{
	public:
		T f(T input){ return 1/(1+exp(-input)); }
	};
	template <typename T>
	class ThresholdAF:public Activationfunction<T>{
	public:
		T f(T input){ if(input>0)return 1; return 0; }
	};


	template <typename T,template<typename> class S>
	class AF:public S<T>{
		using S<T>::f;
	public:
		AF(){S::IsAF();}
		T f(T input){return f(input);}
	};


}
}

#endif//MFL_AI_ACTIVATIONFUNCTIONS_INCLUDED