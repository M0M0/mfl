#ifndef MFL_GL_SHADER_INCLUDED
#define MFL_GL_SHADER_INCLUDED

#include "MFL_GL_Instances.hpp"
#include <string>
//#include "glew_wrap.h"


namespace MFL{
namespace GL{

	class Shader:virtual public Base{
	public:
		Shader():Base(){
		}
		~Shader(){
			this->Delete();
		}

		void Create(GLenum type){
			if(!_set)
				_id = glCreateShader(type);
			Singleton<Instances<Shader>>::getInstance()->Register(this);
		}
		void Delete(){
			if(_set){
				glDeleteShader(_id);
				Singleton<Instances<Shader>>::getInstance()->Update();
			}
		}

		void Compile(){
			glCompileShader(_id);
		}
		static void ReleaseCompiler(){
			glReleaseShaderCompiler();
		}

		void Source(std::string str){
			int clength = static_cast<int>(str.size()+1);
			const char *cstr = str.c_str();
			glShaderSource(_id,1,&cstr,&clength);
		}
		void Binary(GLenum binaryformat,GLvoid *binary,GLsizei length){
			glShaderBinary(1,&_id,binaryformat,binary,length);
		}

		GLint Get(GLenum pname){
			GLint param;
			glGetShaderiv(_id,pname,&param);
			return param;
		}
		std::string GetInfoLog(){
			GLint l = Get(GL_INFO_LOG_LENGTH);
			char* log = new char[l];
			glGetShaderInfoLog(_id,l,nullptr,log);
			std::string str(log);
			delete[] log;
			return str;
		}

		//glShaderBinary

		GLboolean Is(){
			return glIsShader(_id);
		}
	};
}
}
#endif//MFL_GL_SHADER_INCLUDED