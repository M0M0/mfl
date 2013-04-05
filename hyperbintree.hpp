#ifndef MFL_HYPERBINTREE_INCLUDED
#define MFL_HYPERBINTREE_INCLUDED


#include <cassert>
#include "MFL_LinAlg_Vector.hpp"

namespace MFL{

	// Description: Class for generic Binary-, Quad-, Octree implementation
	template <class C, int D, typename T>
	class HyperBinTree {
	private:
		const static int NumOfChilds = D*2;
	public:
		//enum ERROR {	OUTOFBOUND = -2, CHILDUNALLOCATED, SUCCESS };

		HyperBinTree*			_child[NumOfChilds];

		LinAlg::Vector<T,D>		_key;
		C						_value;

		HyperBinTree(LinAlg::Vector<T,D> key,C value):_key(key),_value(value){}
		~HyperBinTree(){
			for(HyperBinTree* ptr=_child[0]; ptr!=_child[NumOfChilds];++ptr)
				delete ptr;
		}
		int InsertChild(LinAlg::Vector<T,D> key,C value){
			int i;
			for(int j=0;j<D;++j)
				key(j)>=_key(j) ? (j+1)*

			assert(i>=0&&i<D);
			if(_child[i]==nullptr)
				_child[i] = new HyperBinTree(key,value);
			else
				_child[i]->InsertChild(key,value);
			return SUCCESS;
		}
		int RemoveChild(int idx){
			assert(idx>=0&&idx<D);
			Child[idx]=nullptr;
			return MFL_SUCCESS;
		}
	
		int NumberOfLeafs(){
			int count = 1;
			for(HyperBinTree* childptr=_child[0];childptr!=_child[NumOfChilds]; ++childptr)
				if(childptr != nullptr)
					count += childptr->NumberOfLeafs();
			
			return count;
		}
	};

}; //namespace MFL
#endif //MFL_HYPERBINTREE_INCLUDED