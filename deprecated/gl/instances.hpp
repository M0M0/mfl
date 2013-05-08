#ifndef MFL_GL_INSTANCES_INCLUDED
#define MFL_GL_INSTANCES_INCLUDED

#include "MFL_GL_Base.hpp"
#include "MFL_Singleton.hpp"
#include <map>

namespace MFL{
namespace GL{

	template <class T>
	class Instances{
	private:
		std::map<GLuint,T*> _inst;
		void RegisterNoCheck(T* newinst){

			if(	_inst.find((*newinst)())==_inst.end())
				_inst[(*newinst)()] = newinst;
		}
	public:
		Instances():_inst(){
			static_assert(std::is_base_of<GL::Base,T>::value,"T not Derived of GL::Base");
		}
		void Update(){
			for(std::map<GLuint,T*>::iterator it = _inst.begin();
				it != _inst.end(); ++it)
				// value has changed!
				if( it->second==nullptr||it->second->Is()==GL_FALSE){
					_inst.erase(it);
				}
				else if( it->first != (*(it->second))() ){
					T* tmp = it->second;
					_inst.erase(it);
					RegisterNoCheck(tmp);
				}
		}
		void Register(T* newinst){
			Update();
			RegisterNoCheck(newinst);
		}
		T* Find(GLuint id){
			std::map<GLuint,T&>::iterator it = _inst.find(id);

			if(it!=_inst.end()&&it->second!=nullptr)
				return it;

			return nullptr;
		}
	};

}
}

#endif//MFL_GL_INSTANCES_INCLUDED