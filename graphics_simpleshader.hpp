//====================================================================================
// SimpleShader Class:
//	Takes care of everything shader
//      Can only use vertex, geometry and fragment programs
//====================================================================================
#ifndef MFL_GRAPHICS_SIMPLESHADER_INCLUDED
#define MFL_GRAPHICS_SIMPLESHADER_INCLUDED

#include "graphics_common.hpp"
#include "exception.hpp"

#include <GL/glew.h>

#include <map>
#include <vector>
#include <cstring>

GRAPHICS_BEGIN

class SimpleShader{
public:
    SimpleShader()
        :attrib_(),uniform_(),
         num_attribs_(0),
         program_(0),
         vertex_shader_(0),fragment_shader_(0),geometry_shader_(0){}
    ~SimpleShader(){
        _deleteShader(vertex_shader_);
        _deleteShader(geometry_shader_);
        _deleteShader(fragment_shader_);

        _deleteProgram();
    }

    //================================================================================
    // will bind attributes to incrementing uint
    //================================================================================
    void setAttrib
    (
        char const * name
    )
    {
        attrib_[name] = num_attribs_;
        glBindAttribLocation(program,n_attribs,name);
        ++num_attribs_;
    }
    GLuint getAttrib
    (
        char const * name
    )
    const
    { return attrib_[name]; }

    void resetAttribs()
    {
        attrib_.clear();
        num_attribs_ = 0;
    }

    //================================================================================
    // will throw exception if name is not found
    //================================================================================
    void setUniform
    (
        char const * name
    )
    {
        GLint location = glGetUniformLocation(program,name);
        if( location == -1 )
                THROW_RT_ERR("Could not find uniform with name \'"+name+"\' or name is reserved.");

        uniform_[name] = location;
    }
    GLint getUniform
    (
        char const * name
    )
    const
    { return uniform_[name]; }

    void resetUniforms()
    { uniform_.clear(); }

    //================================================================================
    // methods to aquire shaders from source
    //================================================================================
    void addShader_VS
    (
        char const * shader_source
    )
    { _createShaderFromSource( shader_source, GL_VERTEX_SHADER, vertex_shader_); }
    
    void addShader_GS
    (
        char const * shader_source
    )
    { _createShaderFromSource( shader_source, GL_GEOMETRY_SHADER, geometry_shader_); }
    
    void addShader_FS
    (
        char const * shader_source
    )
    { _createShaderFromSource( shader_source, GL_FRAGMENT_SHADER, fragment_shader_); }

    //================================================================================
    // method to bind shaders to program 
    // shaders must first be aquired via 'addShader_*'
    //================================================================================	
    void link()
    {
        if(program_ > 0) // maybe a throw instead
                _deleteProgram();

        program_ = glCreateProgram();

        if(vertex_shader_ > 0)
            glAttachShader(program,vertex);
        if(geometry_shader_ > 0)
            glAttachShader(program,geometry);
        if(fragment_shader_ > 0)
            glAttachShader(program,fragment);

        glLinkProgram(program);
        _programLinkCheck();
    }
    //================================================================================
    // method to activate program
    // if debugging is enabled it will also validate the program 
    //================================================================================	
    void use()
    {
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
    void _createShaderFromSource
    (
        char const * source, 
        GLenum shader_type,
        GLuint shader_id
    )
    {
        if(shader_id)// throw exception
                _deleteShader(shader_id);

        shader_id = glCreateShader(shader_type);
        GLint source_size = ::strlen(source);
        glShaderSource(shader_id,1,&shader_source,&source_size);
        glCompileShader(shader_id);
        _shaderCompileCheck(shader_id);
    }
    void _shaderCompileCheck
    (
        GLuint shader_id
    )
    {
        GLint success = _shader_glGet(shader_id,GL_COMPILE_STATUS);
        if( success != GL_TRUE )
                _shaderInfoLog_THROW(shader_id);
    }

    void _shaderInfoLog_THROW
    (
        GLuint shader_id
    )
    {
        GLint log_size = _shader_glGet(shader_id,GL_INFO_LOG_LENGTH);
        std::vector<GLchar> log(log_size);
        glGetShaderInfoLog(shader_id,log_size,nullptr,log.data());

        THROW_RT_ERR(std::string("GL Shader Error : ")+log.data());
    }

    void _deleteShader
    (
        GLuint shader_id
    )
    {
        glDeleteShader(shader_id);
        shader_id = 0;
    }

    GLint _shader_glGet
    (
        GLuint shader_id,
        GLenum what
    )
    {
        GLint param;
        glGetShaderiv(shader_id,what,&param);
        return param;
    }

    //================================================================================
    // Hidden program functions
    //================================================================================
    void _programLinkCheck()
    {
        GLint success = _program_glGet(program_,GL_LINK_STATUS);
        if( success != GL_TRUE )
                _programInfoLog_THROW();
    }
    void _programValidateCheck()
    {
        GLint success = _program_glGet(program_,GL_VALIDATE_STATUS);
        if( success != GL_TRUE )
                _programInfoLog_THROW();
    }

    void _programInfoLog_THROW()
    {
        GLint log_size = _program_glGet(program_,GL_INFO_LOG_LENGTH);
        std::vector<GLchar> log( log_size );
        glGetShaderInfoLog(program,log_size,nullptr,log.data());

        THROW_RT_ERR(std::string("GL Program Error : ")+log.data());
    }

    void _deleteProgram()
    {
        glDeleteProgram(program_);
        program_ = 0;
    }

    GLint _program_glGet
    (
        GLuint program_id,
        GLenum what
    )
    {
        GLint param;
        glGetProgramiv(program_id,what,&param);
        return param;
    }



    //================================================================================
    // members
    //================================================================================
private:
    std::map<char const *,GLuint>       attrib_; 
    std::map<char const *,GLint>	uniform_;

    unsigned int num_attribs_;

    GLuint program_;

    GLuint vertex_shader_;
    GLuint fragment_shader_;
    GLuint geometry_shader_;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SIMPLESHADER_INCLUDED