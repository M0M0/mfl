#ifndef MFL_GL_UTILITYSHADERS_INCLUDED
#define MFL_GL_UTILITYSHADERS_INCLUDED

#include "MFL_GL_Program.hpp"


#include <iostream>
#include <cassert>
#include <memory>
#include "MFL_Exception.hpp"

namespace MFL{
namespace GL{

#define STRINGIFY(x) #x
#define GLVERSION_STRING "#version 420\n"

#define GL_STRINGIFY(x) GLVERSION_STRING STRINGIFY(x)

	// basic flatshader pair

	// vertexshader
	// inputs:  - vVertex / vec4
	// uniforms:
	//		    - mMvp    / mat4
	const char*	basicflatshader_vertex = 
		GL_STRINGIFY(
		uniform mat4 mMvp;
		in      vec4 vVertex;
		void main(){
			gl_Position = mMvp*vVertex;
		}
		);
	// fragmentshader
	// uniforms:
	//          - vColor  / vec4
	const char* basicflatshader_fragment =
		GL_STRINGIFY(
		uniform vec4 vColor;
		out		vec4 color_out;
		void main(){
			color_out = vColor;
		}
		);

#ifndef NDEBUG
#define GL_CHECK(obj,x) \
	if( GL_FALSE == (obj).Get(x) ){\
		throw std::runtime_error((obj).GetInfoLog());\
	}
#else
#define GL_CHECK(obj,x)
#endif
	// TODO change to variable arguments
	// ProgArgs: 1 - vVertex | 2 - 1+vNormal | 3 - 2+vColor (just a quick fix!)
	// varargs may be more apropriate!
	MFL::GL::Program VERTFRAG_ShaderProgram(const char* vertstring,const char* fragstring,int ProgArgs){
		MFL::GL::Program program;
		MFL::GL::Shader vert, frag;

		vert.Create(GL_VERTEX_SHADER);
		vert.Source(vertstring);
		vert.Compile();
		if(vert.Get(GL_COMPILE_STATUS)!=GL_TRUE)
			{
			
			GLint logsize = vert.Get(GL_INFO_LOG_LENGTH);
			std::unique_ptr<GLchar> log(new char[logsize]); // needed so no memory leakage occurs!
			glGetShaderInfoLog(vert(),logsize,nullptr,log.get());

			THROW_RT_ERR(
				std::string("Could not compile shader - Error: ")
				+log.get()
				);
			}
		frag.Create(GL_FRAGMENT_SHADER);
		frag.Source(fragstring);
		frag.Compile();
		if(frag.Get(GL_COMPILE_STATUS)!=GL_TRUE)
			{
			
			GLint logsize = frag.Get(GL_INFO_LOG_LENGTH);
			std::unique_ptr<GLchar> log(new char[logsize]); // needed so no memory leakage occurs!
			glGetShaderInfoLog(frag(),logsize,nullptr,log.get());

			THROW_RT_ERR(
				std::string("Could not compile shader - Error: ")
				+log.get()
				);
			}

		program.Create();
		program.AttachShader(vert);
		program.AttachShader(frag);


		switch(ProgArgs){
		case 3:
			program.BindAttributeLocation(MFL::GL::Color, "vColor" );
		case 2:
			program.BindAttributeLocation(MFL::GL::Normal,"vNormal");
		case 1:
			program.BindAttributeLocation(MFL::GL::Vertex,"vVertex");
			break;
		default:
			;
		}
		glBindFragDataLocation(program(),0,"color_out");

		program.Link();
		GL_CHECK(program,GL_LINK_STATUS);

		program.Validate();
		GL_CHECK(program,GL_VALIDATE_STATUS);
		


		return program;
	}
#undef GL_CHECK


}
}

#endif//MFL_GL_UTILITYSHADERS_INCLUDED