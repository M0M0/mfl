#ifndef MFL_SHADER_INCLUDED
#define MFL_SHADER_INCLUDED

#include "MFL_Graphics_Common.hpp"
#include "MFL_Exception.hpp"
#include <GL\glew.h>

#include <string>
#include <memory>
#include <map>

GRAPHICS_BEGIN

#define GLGET(type,id,what) \
	[](GLuint _id,GLenum _what)->GLint{\
		GLint param;\
		::glGet##type##iv(_id,_what,&param);\
		return param;\
	}(id,what)

namespace gl{
	void ShaderCompileCheck(GLint shader){
		if(GLGET(Shader,shader,GL_COMPILE_STATUS)==GL_TRUE)
			return;

		GLint logsize = GLGET(Shader,shader,GL_INFO_LOG_LENGTH);
		std::unique_ptr<GLchar[]> log(new GLchar[logsize]); 
		glGetShaderInfoLog(shader,logsize,nullptr,log.get());

		THROW_RT_ERR(
			std::string("Could not compile shader - Error: ")
			+log.get()
			);
	}

	std::string ProgramInfoLog(GLint program)
	{
		GLint logsize = GLGET(Program,program,GL_INFO_LOG_LENGTH);
		std::unique_ptr<GLchar[]> log(new GLchar[logsize]);
		glGetProgramInfoLog(program,logsize,nullptr,log.get());
		return std::string(log.get());
	}

	void ProgramLinkCheck(GLint program){
		if(GLGET(Program,program,GL_LINK_STATUS)==GL_TRUE)
			return;

		THROW_RT_ERR(
			std::string("Could not link program - Error: ")
			+ProgramInfoLog(program)
			);
	}

	void ProgramValidate(GLint program){
		glValidateProgram(program);

		if(GLGET(Program,program,GL_VALIDATE_STATUS)==GL_TRUE)
			return;

		THROW_RT_ERR(
			std::string("Could not validate program - Error: ")
			+ProgramInfoLog(program)
			);
	}

	// with glsl shaders using location
	GLuint GetProgram_VertexFragmentShader(const char* vertex_source,const char* fragment_source){
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		GLint Size = ::strlen(vertex_source);
		glShaderSource(vShader,1,&vertex_source,&Size);
		glCompileShader(vShader);
		//check for errors
		ShaderCompileCheck(vShader);


		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		Size = strlen(fragment_source);
		glShaderSource(fShader,1,&fragment_source,&Size);
		glCompileShader(fShader);
		//check for errors
		ShaderCompileCheck(fShader);

		GLuint program = glCreateProgram();
		glAttachShader(program,vShader);
		glAttachShader(program,fShader);

		glLinkProgram(program);
		ProgramLinkCheck(program);

#ifdef _DEBUG
		ProgramValidate(program);
#endif//_DEBUG

		return program;
	}

	struct ProgramHandles{
		// program handle
		GLint prog;
		// Uniform handles
		std::map<char*,GLint> Uniform;
	};
}

GRAPHICS_END


#endif//MFL_SHADER_INCLUDED