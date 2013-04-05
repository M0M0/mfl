#ifndef MFL_INSTANCEREGISTER_INCLUDED
#define MFL_INSTANCEREGISTER_INCLUDED

#include <vector>

#define MFL_IR_MEMBER(classname)	MFL::InstanceRegister<classname> IR
#define MFL_IR_CONSTRUCT			IR.add(this)
#define MFL_IR_DESTRUCT				IR.remove(this)

namespace MFL{

template <class T>
class InstanceRegister{
	typedef std::vector<T*>		PtrVec_t;
	typedef PtrVec_t			&PtrVecRef_t;
public:
	static PtrVecRef_t  getInstances(){ return _inst; }
	static void			add	  (T* x){ _inst.push_back(x); }
	static void			remove(T* x)
		{ _inst.erase(std::find(_inst.cbegin(),_inst.cend(),x)); }
private:
	static PtrVec_t _inst;
	friend T;
};
template <class T>
std::vector<T*> InstanceRegister<T>::_inst = std::vector<T*>();
}

/*
saved for further testing!

template <class Derived,class Base>
struct Base_t:public Base{
	MFL_IR_MEMBER(Derived);

	Base_t():Base()		{IR.add(reinterpret_cast<Derived*>(this));}
	virtual ~Base_t()	{IR.remove(reinterpret_cast<Derived*>(this));}
};
struct Base{
	int x;
	Base():x(0){}
};

struct Derived:public Base_t<Derived,Base>{
	int y;
	Derived():Base_t<Derived,Base>(),y(1){}
	~Derived(){	}
};

struct Base2:public Base_t<Base,Base>{
	Base2():Base_t<Base,Base>(){}
};


int main(){
	Derived* x = new Derived,y;
	Base2 k;

	x->y = 0;
	x->x = 1;

	auto& DInst = MFL::InstanceRegister<Derived>::getInstances();
	auto& BInst = MFL::InstanceRegister<Base>::getInstances();
	std::cout << std::hex << reinterpret_cast<unsigned int>(DInst[0]) << '\t' << reinterpret_cast<unsigned int>(x) << '\n';

	std::cout << "Derived N:" << DInst.size() << '\n';
	std::cout << "Base    N:" << BInst.size() << '\n';


	delete x;

	std::cout << "Derived N:" << DInst.size() << '\n';
	std::cout << "Base    N:" << BInst.size() << '\n';


	return 0;
}

*/

#endif//MFL_INSTANCEREGISTER_INCLUDED