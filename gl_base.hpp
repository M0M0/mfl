#ifndef MFL_GL_BASE_INCLUDED
#define MFL_GL_BASE_INCLUDED

#include <GL\glew.h>

namespace MFL{
namespace GL{

	class Base{
	protected:
		GLuint _id;
		bool  _set;
	public:
		Base():_id(0),_set(false){}

		GLuint operator()(){
			return _id;
		}
		virtual GLboolean Is() = 0;
	};


}
}

#endif//MFL_GL_BASE_INCLUDED