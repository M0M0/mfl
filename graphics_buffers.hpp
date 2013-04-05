//======================================================================================
// A buffer class for all things concerning gpu memory
// TODO: - replace 'replace' function by transform feedback using gpu
//		 - maybe a method to iterate through scenenodes (?)
//======================================================================================
#ifndef MFL_GRAPHICS_BUFFERS_INCLUDED
#define MFL_GRAPHICS_BUFFERS_INCLUDED

#include "MFL_Graphics_Common.hpp"

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <xutility>

// for bigger buffer switch to GLuint! ( now using GLushort )
#define BUFFER_SIZE (1<<16)
#define N_BUFFERS	4

GRAPHICS_BEGIN

class Buffers{
	enum Buffer{ Position, Normal, Color, Index };

public:
	Buffers():_offset(0),_indexoffset(0){}
	~Buffers(){
		glDeleteBuffers(N_BUFFERS,_BufferHandle);
		glDeleteVertexArrays(1,&_VAHandle);
	}

	//======================================================================================
	// Sets up Buffers
	//======================================================================================
	void Generate(){
		_offset		 = 0;
		_indexoffset = 0;

		glGenBuffers(N_BUFFERS,_BufferHandle);
		glGenVertexArrays(1,&_VAHandle);

		_initBuffer(Position,GL_ARRAY_BUFFER,sizeof(glm::vec3),GL_STATIC_DRAW);
		_initBuffer(  Normal,GL_ARRAY_BUFFER,sizeof(glm::vec3),GL_STATIC_DRAW);
		_initBuffer(   Color,GL_ARRAY_BUFFER,sizeof(glm::vec4),GL_STATIC_DRAW);

		_initBuffer(   Index,GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort),GL_STATIC_DRAW);

		glBindVertexArray(_VAHandle);

		glEnableVertexAttribArray(Position);
		glEnableVertexAttribArray(Normal);
		glEnableVertexAttribArray(Color);
		
		_bindVertexPointer( Position,3,GL_FLOAT,GL_FALSE );
		_bindVertexPointer(   Normal,3,GL_FLOAT,GL_TRUE  );
		_bindVertexPointer(    Color,4,GL_FLOAT,GL_TRUE  );

	}

	template <typename p_iter,typename n_iter,
		typename c_iter,typename i_iter>
	std::pair<size_t,size_t> append( 
		p_iter first_p,p_iter last_p,
		n_iter first_n,c_iter first_c,
		i_iter first_i,i_iter last_i)
	{
		static_assert(is_continuous_iterator<p_iter>::value,"Position is not layed out in continuous memory.");
		static_assert(is_continuous_iterator<n_iter>::value,"Normal is not layed out in continuous memory.");
		static_assert(is_continuous_iterator<c_iter>::value,"Color is not layed out in continuous memory.");
		static_assert(is_continuous_iterator<i_iter>::value,"Index is not layed out in continuous memory.");

		size_t diff		 = std::distance(first_p,last_p);
		size_t indexdiff = std::distance(first_i,last_i);

		assert(		 _offset +      diff <= BUFFER_SIZE ); // buffer overflow!
		assert( _indexoffset + indexdiff <= BUFFER_SIZE ); // indexbuffer overflow!

		_insertBufferData(Position,&*first_p,_offset,sizeof(glm::vec3),diff);
		_insertBufferData(Normal  ,&*first_n,_offset,sizeof(glm::vec3),diff);
		_insertBufferData(Color   ,&*first_c,_offset,sizeof(glm::vec4),diff);

		_insertIndexBufferData(&*first_i,_indexoffset,sizeof(GLushort),indexdiff);

		// get offset 
		auto offset_pair = getoffset();
		
		// increase offset
		_offset		 += diff;
		_indexoffset += indexdiff;

		// return preincremented offset
		return offset_pair;
	}

	template <typename p_iter,typename n_iter,
		typename c_iter,typename i_iter> 
		// not safe check boundaries yourself!
	void replace(
		size_t offset ,size_t indexoffset, 
		p_iter first_p,p_iter last_p,
		n_iter first_n,c_iter first_c,
		i_iter first_i,i_iter last_i)
	{
		static_assert(is_continuous_iterator<p_iter>::value,"Position is not layed out in continuous memory.");
		static_assert(is_continuous_iterator<n_iter>::value,"Normal is not layed out in continuous memory.");
		static_assert(is_continuous_iterator<c_iter>::value,"Color is not layed out in continuous memory.");
		static_assert(is_continuous_iterator<i_iter>::value,"Index is not layed out in continuous memory.");

		size_t diff		 = std::distance(first_p,last_p);
		size_t indexdiff = std::distance(first_i,last_i);

		assert(		 offset +      diff <= BUFFER_SIZE ); // buffer overflow!
		assert( indexoffset + indexdiff <= BUFFER_SIZE ); // indexbuffer overflow!

		// maybe throw something instead
		if( offset + diff > _offset ) 
			_offset = offset + diff;
		if( indexoffset + indexdiff > _indexoffset )
			_indexoffset = indexoffset + indexdiff;

		_insertBufferData(Position,&*first_p,offset,sizeof(glm::vec3),diff);
		_insertBufferData(Normal  ,&*first_n,offset,sizeof(glm::vec3),diff);
		_insertBufferData(Color   ,&*first_c,offset,sizeof(glm::vec4),diff);

		_insertIndexBufferData(&*first_i,indexoffset,sizeof(GLushort),indexdiff);
	}
	void replace(
		size_t offset,size_t indexoffset,
		glm::mat4& tf)
	{
		// copy data from position and normal buffer
		// to TRANSFORM_FEEDBACK_BUFFER

		// set input to transform_feedback_buffer
		// set output to former position in old buffer

		// use transform program
		
		// set uniform to tf

		// run gldrawarrays
	}

	std::pair<size_t,size_t> getoffset() const
	{
		return std::pair<size_t,size_t>(_offset,_indexoffset);
	}

	void DrawIndexed(GLenum mode = GL_TRIANGLES)
	{
		glBindVertexArray(_VAHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_BufferHandle[Index]);
		glDrawElements(mode,static_cast<GLsizei>(_indexoffset),GL_UNSIGNED_SHORT,nullptr);
	}
	void Draw		(GLenum mode = GL_TRIANGLES)
	{
		glBindVertexArray(_VAHandle);
		glDrawArrays(mode,0,static_cast<GLsizei>(_offset));
	}

private:
	//======================================================================================
	// Private methods
	//======================================================================================
	void _initBuffer(
		Buffer buffer,
		GLenum target,
		size_t element_size,
		GLenum usage_mode)
	{
		glBindBuffer(target,_BufferHandle[buffer]);
		glBufferData(target,BUFFER_SIZE*element_size,nullptr,usage_mode);
	}

	void _bindVertexPointer(
		Buffer buffer,
		GLint  element_number,
		GLenum element_type,
		GLenum normalized
		)
	{
		glBindBuffer(GL_ARRAY_BUFFER,_BufferHandle[buffer]);
		glVertexAttribPointer(buffer,element_number,element_type,normalized,0,0);
	}

	void _insertBufferData(
		Buffer buffer,
		const GLvoid* data_ptr,
		size_t offset,
		size_t element_size,
		size_t set_size		)
	{
		glBindBuffer(GL_ARRAY_BUFFER,_BufferHandle[buffer]);
		glBufferSubData(GL_ARRAY_BUFFER,element_size*offset,element_size*set_size,data_ptr);
	}
	void _insertIndexBufferData(
		const GLvoid* data_ptr,
		size_t offset,
		size_t element_size,
		size_t set_size			)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_BufferHandle[Index]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,element_size*offset,element_size*set_size,data_ptr);
	}

	template <typename T>
	struct is_continuous_iterator:
			std::is_same<
				typename std::iterator_traits<T>::iterator_category,
				std::random_access_iterator_tag
			>
	{};

private:
	//======================================================================================
	// Members
	//======================================================================================
	GLuint _BufferHandle[N_BUFFERS];
	GLuint _VAHandle;

	size_t _offset, _indexoffset;
};

GRAPHICS_END

#undef N_BUFFERS
#undef BUFFER_SIZE

#endif//MFL_GRAPHICS_BUFFERS_INCLUDED