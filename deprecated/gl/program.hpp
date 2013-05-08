#ifndef MFL_GL_PROGRAM_INCLUDED
#define MFL_GL_PROGRAM_INCLUDED

#include "MFL_GL_Instances.hpp"
#include "MFL_GL_Shader.hpp"
#include "MFL_GL_VertexArray.hpp"
#include <string>

namespace MFL{
namespace GL{

	class Program:virtual public Base{
	public:
		Program():Base(){}
		~Program(){
			this->Delete();
		}
		void Create(){
			if(!_set){
				_id = glCreateProgram();
				Singleton<Instances<Program>>::getInstance()->Register(this);
			}
		}
		void Delete(){
			if(_set){
				_set = !_set;
				glDeleteProgram(_id);
			}
			//Singleton<Instances<Program>>::getInstance()->Update();
		}
		GLboolean Is(){
			return glIsProgram(_id);
		}

		void AttachShader(GL::Shader shader){
			glAttachShader(_id,shader());
		}
		void DetachShader(GL::Shader shader){
			glDetachShader(_id,shader());
		}

		void Link(){
			glLinkProgram(_id);
		}
		void Use(){
			glUseProgram(_id);
		}
		void Validate(){
			glValidateProgram(_id);
		}

		void BindAttributeLocation(VertexAttribute index,const GLchar* name){
			glBindAttribLocation(_id,index,name);
		}
		GLint Parameter(GLenum pname){
			GLint value;
			glProgramParameteri(_id,pname,value);
			return value;
		}
		GLint GetUniformLocation(const char* name){
			return glGetUniformLocation(_id,name);
		}

		GLint Get(GLenum param){
			GLint pdata;
			glGetProgramiv(_id,param,&pdata);
			return pdata;
		}
		std::string GetInfoLog(){
			GLint l = Get(GL_INFO_LOG_LENGTH);
			char* log = new char[l];
			glGetProgramInfoLog(_id,l,nullptr,log);
			std::string str(log);
			delete[] log;
			return str;
		}

	};

}
}

#endif//MFL_GL_PROGRAM_INCLUDED