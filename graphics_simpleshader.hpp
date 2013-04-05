//====================================================================================
// SimpleShader Class:
//	 Takes care of everything shader
// 
// Discussion:
//   Should i use std::vector<char> instead of std::unique_ptr<char[]> ?
//====================================================================================
#ifndef MFL_GRAPHICS_SIMPLESHADER_INCLUDED
#define MFL_GRAPHICS_SIMPLESHADER_INCLUDED

#include "MFL_Graphics_Common.hpp"
#include "MFL_Exception.hpp"

#include <GL\glew.h>

#include <map>
#include <memory>

GRAPHICS_BEGIN

class SimpleShader{
	typedef char const * const		const_str;

	//================================================================================
	// following typedefs are just for the sake of explicity
	//================================================================================
	typedef char const *			  const_str_weak;
	typedef std::unique_ptr<GLchar[]> GLchar_ptr;

	typedef GLuint	Shader;
	typedef GLuint	Program;
	typedef GLuint	Attribute;

	typedef GLint	UniformLocation;
public:
	SimpleShader()
		:n_attribs(0),attrib(),uniform(),
		 program(0),
		 vertex(0),fragment(0),geometry(0){}
	~SimpleShader(){
		_deleteShader(vertex);
		_deleteShader(fragment);
		_deleteShader(geometry);

		_deleteProgram();
	}

	//================================================================================
	// will bind attributes to incrementing uint
	//================================================================================
	void   setAttrib(const_str name){
		attrib[name] = n_attribs;
		glBindAttribLocation(program,n_attribs,name);
		++n_attribs;
	}
	GLuint getAttrib(const_str name){
		return attrib[name];
	}

	//================================================================================
	// will throw exception if name is not found
	//================================================================================
	void  setUniform(const_str name){
		UniformLocation location = glGetUniformLocation(program,name);
		if( location == -1 )
			THROW_RT_ERR("Could not find uniform with name \'"+name+"\' or name is reserved.");

		uniform[name] = location;
	}
	GLint getUniform(const_str name){
		return uniform[name];
	}

	//================================================================================
	// methods to aquire shaders from source
	//================================================================================
	void addShader_VS(const_str shader){
		_createShaderFromSource(shader,GL_VERTEX_SHADER,vertex);
	}
	void addShader_GS(const_str shader){
		_createShaderFromSource(shader,GL_GEOMETRY_SHADER,geometry);
	}
	void addShader_FS(const_str shader){
		_createShaderFromSource(shader,GL_FRAGMENT_SHADER,fragment);
	}

	//================================================================================
	// method to bind shaders to program 
	// shaders must first be aquired via 'addShader_*'
	//================================================================================	
	void link(){
		if(program)
			_deleteProgram();

		program = glCreateProgram();

		if(vertex)
			glAttachShader(program,vertex);
		if(geometry)
			glAttachShader(program,geometry);
		if(fragment)
			glAttachShader(program,fragment);

		glLinkProgram(program);
		_programLinkCheck();
	}
	//================================================================================
	// method to activate program
	// if debugging is enabled it will also validate the program 
	//================================================================================	
	void use(){
#ifdef _DEBUG
		glValidateProgram(program);
		_programValidateCheck();
#endif
		glUseProgram(program);
	}

private:
	//================================================================================
	// Hidden shader functions
	//================================================================================
	void _createShaderFromSource(const_str_weak source,GLenum type,Shader& id){
		if(id)// throw exception
			_deleteShader(id);

		id = glCreateShader(type);
		GLint source_size = ::strlen(source);
		glShaderSource(id,1,&source,&source_size);
		glCompileShader(id);
		_shaderCompileCheck(id);
	}
	void _shaderCompileCheck(Shader& id){
		GLint success = _shader::glGet(id,GL_COMPILE_STATUS);
		if( success != GL_TRUE )
			_shaderInfoLog_THROW(id);
	}

	void _shaderInfoLog_THROW(Shader& id){
		GLint log_size = _shader::glGet(id,GL_INFO_LOG_LENGTH);
		GLchar_ptr log( new GLchar[log_size] );
		glGetShaderInfoLog(id,log_size,nullptr,log.get());

		THROW_RT_ERR(std::string("GL Shader Error : ")+log.get());
	}

	void _deleteShader(Shader& id){
		glDeleteShader(id);
		id = 0;
	}

	struct _shader{
		static GLint glGet(GLuint& id,GLenum what){
			GLint param;
			glGetShaderiv(id,what,&param);
			return param;
		}
	};

	//================================================================================
	// Hidden program functions
	//================================================================================
	void _programLinkCheck(){
		GLint success = _program::glGet(program,GL_LINK_STATUS);
		if( success != GL_TRUE )
			_programInfoLog_THROW();
	}
	void _programValidateCheck(){
		GLint success = _program::glGet(program,GL_VALIDATE_STATUS);
		if( success != GL_TRUE )
			_programInfoLog_THROW();
	}

	void _programInfoLog_THROW(){
		GLint log_size = _program::glGet(program,GL_INFO_LOG_LENGTH);
		GLchar_ptr log( new GLchar[log_size] );
		glGetShaderInfoLog(program,log_size,nullptr,log.get());

		THROW_RT_ERR(std::string("GL Program Error : ")+log.get());
	}

	void _deleteProgram(){
		glDeleteProgram(program);
		program = 0;
	}

	struct _program{
		static GLint glGet(GLuint& id,GLenum what){
			GLint param;
			glGetProgramiv(id,what,&param);
			return param;
		}
	};


	//================================================================================
	// members
	//================================================================================
private:
	GLuint n_attribs;
	std::map<const_str,GLuint>	attrib; 
	std::map<const_str,GLint>	uniform;

	Program program;

	Shader	vertex;
	Shader	fragment;
	Shader	geometry;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SIMPLESHADER_INCLUDED