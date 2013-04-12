//==============================================================================
// SimpleShader Class:
//	Takes care of everything shader
//      Can only use vertex, geometry and fragment programs
//==============================================================================
#ifndef MFL_GRAPHICS_SIMPLESHADER_INCLUDED
#define MFL_GRAPHICS_SIMPLESHADER_INCLUDED

#include <cstring>
#include <map>
#include <vector>

#include <GL/glew.h>

#include "graphics_common.hpp"
#include "exception.hpp"

GRAPHICS_BEGIN
/// \class SimpleShader
class SimpleShader{
public:
  SimpleShader()
    : attrib_(),
      uniform_(),
      num_attribs_(0),
      program_(0),
      vertex_shader_(0),
      fragment_shader_(0),
      geometry_shader_(0) {
  }
  ~SimpleShader() {
    _DeleteShader(vertex_shader_);
    _DeleteShader(geometry_shader_);
    _DeleteShader(fragment_shader_);
    _DeleteProgram();
  }
  //==========================================================================
  // will bind attributes to incrementing uint
  //==========================================================================
  void SetAttrib(char const * name) {
    attrib_[name] = num_attribs_;
    glBindAttribLocation(program_,num_attribs_++,name);
  }
  GLuint GetAttrib(char const * name) {
    return attrib_[name];
  }
  void ResetAttribs() {
    attrib_.clear();
    num_attribs_ = 0;
  }
  //==========================================================================
  // will throw exception if name is not found
  //==========================================================================
  void SetUniform(char const * name) {
    GLint location = glGetUniformLocation(program_,name);
    if (location == -1)
      THROW_RT_ERR("Could not find uniform with name \'"+name+
		   "\' or name is reserved.");
    uniform_[name] = location;
  }
  GLint GetUniform(char const * name) {
    return uniform_[name];
  }
  void ResetUniforms() {
    uniform_.clear();
  }
  //==========================================================================
  // methods to aquire shaders from source
  //==========================================================================
  void AddShaderVS(char const * shader_source) {
    _CreateShaderFromSource(shader_source,GL_VERTEX_SHADER,vertex_shader_);
  }
  void AddShaderGS(char const * shader_source) {
    _CreateShaderFromSource(shader_source,GL_GEOMETRY_SHADER,geometry_shader_);
  } 
  void AddShaderFS(char const * shader_source) {
    _CreateShaderFromSource(shader_source,GL_FRAGMENT_SHADER,fragment_shader_);
  }
  //==========================================================================
  // method to bind shaders to program 
  // shaders must first be aquired via 'addShader_*'
  //==========================================================================
  void Link() {
    if (program_ > 0) _DeleteProgram(); // maybe a throw instead
    program_ = glCreateProgram();
    if (vertex_shader_   > 0) glAttachShader(program_,vertex_shader_);
    if (geometry_shader_ > 0) glAttachShader(program_,geometry_shader_);
    if (fragment_shader_ > 0) glAttachShader(program_,fragment_shader_);
    glLinkProgram(program_);
    _ProgramLinkCheck();
  }
  //==========================================================================
  // method to activate program
  // if debugging is enabled it will also validate the program 
  //==========================================================================	
  void Use() {
#ifdef _DEBUG
    glValidateProgram(program_);
    _ProgramValidateCheck();
#endif
    glUseProgram(program_);
  }
private:
  //==========================================================================
  // Hidden shader functions
  //==========================================================================
  void _CreateShaderFromSource(char const * source,GLenum shader_type,
			       GLuint& shader_id) {
    if (shader_id > 0) _DeleteShader(shader_id); // throw exception
    shader_id = glCreateShader(shader_type);
    GLint source_size = strlen(source);
    glShaderSource(shader_id,1,&source,&source_size);
    glCompileShader(shader_id);
    _ShaderCompileCheck(shader_id);
  }
  void _ShaderCompileCheck(GLuint shader_id) {
    GLint success = _ShaderGLGet(shader_id,GL_COMPILE_STATUS);
    if (success != GL_TRUE) _ShaderInfoLogTHROW(shader_id);
  }
  void _ShaderInfoLogTHROW(GLuint shader_id) {
    GLint log_size = _ShaderGLGet(shader_id,GL_INFO_LOG_LENGTH);
    std::vector<GLchar> log(log_size);
    glGetShaderInfoLog(shader_id,log_size,nullptr,log.data());
    THROW_RT_ERR(std::string("GL Shader Error : ")+log.data());
  }
  void _DeleteShader(GLuint shader_id) {
    glDeleteShader(shader_id);
    shader_id = 0;
  }
  GLint _ShaderGLGet(GLuint shader_id,GLenum what) {
    GLint param;
    glGetShaderiv(shader_id,what,&param);
    return param;
  }
  //==========================================================================
  // Hidden program functions
  //==========================================================================
  void _ProgramLinkCheck() {
    GLint success = _ProgramGLGet(program_,GL_LINK_STATUS);
    if (success != GL_TRUE) _ProgramInfoLogTHROW();
  }
  void _ProgramValidateCheck() {
    GLint success = _ProgramGLGet(program_,GL_VALIDATE_STATUS);
    if (success != GL_TRUE) _ProgramInfoLogTHROW();
  }
  void _ProgramInfoLogTHROW() {
    GLint log_size = _ProgramGLGet(program_,GL_INFO_LOG_LENGTH);
    std::vector<GLchar> log( log_size );
    glGetShaderInfoLog(program_,log_size,nullptr,log.data());
    
    THROW_RT_ERR(std::string("GL Program Error : ")+log.data());
  }
  void _DeleteProgram() {
    glDeleteProgram(program_);
    program_ = 0;
  }
  GLint _ProgramGLGet(GLuint program_id,GLenum what) {
    GLint param;
    glGetProgramiv(program_id,what,&param);
    return param;
  }
  //==========================================================================
  // members
  //==========================================================================
  std::map<char const *,GLuint> attrib_; 
  std::map<char const *,GLint> uniform_;
  unsigned int num_attribs_;
  GLuint program_;
  GLuint vertex_shader_;
  GLuint fragment_shader_;
  GLuint geometry_shader_;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SIMPLESHADER_INCLUDED
