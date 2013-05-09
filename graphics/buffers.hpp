//==============================================================================
// A buffer class for all things concerning gpu memory
// TODO: - replace 'replace' function by transform feedback using gpu
//		 - maybe a method to iterate through scene nodes (?)
//==============================================================================
#ifndef MFL_GRAPHICS_BUFFERS_INCLUDED
#define MFL_GRAPHICS_BUFFERS_INCLUDED

#include <cassert>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "common.hpp"

// This Class uses a hardcoded Buffer Limit!
// For bigger buffer switch indexing buffer to GLuint! ( now using GLushort )
#define BUFFER_SIZE (1<<16)
#define N_BUFFERS   4

GRAPHICS_BEGIN

// This class still needs a safe method of initiation.
// Either by using member variable or a factory method.
class Buffers {
  enum Buffer { kPosition, kNormal, kColor, kIndex };
 public:
  Buffers() : offset_(0), indexoffset_(0) {}
  ~Buffers() {
    glDeleteBuffers(N_BUFFERS,buffer_handle_);
    glDeleteVertexArrays(1,&va_handle_);
  }

  // Sets up Buffers
  void Generate() {
    offset_      = 0;
    indexoffset_ = 0;

    glGenBuffers(N_BUFFERS,buffer_handle_);
    glGenVertexArrays(1,&va_handle_);
        
    _InitBuffer(kPosition,GL_ARRAY_BUFFER,sizeof(glm::vec3),GL_STATIC_DRAW);
    _InitBuffer(kNormal,  GL_ARRAY_BUFFER,sizeof(glm::vec3),GL_STATIC_DRAW);
    _InitBuffer(kColor,   GL_ARRAY_BUFFER,sizeof(glm::vec4),GL_STATIC_DRAW);

    _InitBuffer(kIndex,GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLushort),GL_STATIC_DRAW);

    glBindVertexArray(va_handle_);

    glEnableVertexAttribArray(kPosition);
    glEnableVertexAttribArray(kNormal);
    glEnableVertexAttribArray(kColor);
		
    _BindVertexPointer(kPosition,3,GL_FLOAT,GL_FALSE);
    _BindVertexPointer(kNormal,3,GL_FLOAT,GL_TRUE);
    _BindVertexPointer(kColor,4,GL_FLOAT,GL_TRUE);
  }

  // Watch out this function assumes that Position,Normal and Color Buffer
  // are all of the same length!
  template <typename PositionIterator,typename NormalIterator,
	    typename ColorIterator,typename IndexIterator>
  std::pair<size_t,size_t> Append(
      PositionIterator position_first,
      PositionIterator position_last,
      NormalIterator normal_first,
      ColorIterator color_first,
      IndexIterator index_first,
      IndexIterator index_last) {
    //TODO(mojo): Fix this static_assert block!
    /*		static_assert(is_continuous_iterator<p_iter>::value,
     *                    "Position is not laid out in continuous memory.");
     static_assert(is_continuous_iterator<n_iter>::value,
     *                    "Normal is not laid out in continuous memory.");
     static_assert(is_continuous_iterator<c_iter>::value,
     *                    "Color is not laid out in continuous memory.");
     static_assert(is_continuous_iterator<i_iter>::value,
     *                    "Index is not laid out in continuous memory.");*/

    size_t diff	= std::distance(position_first,position_last);
    size_t indexdiff = std::distance(index_first,index_last);

    assert(offset_ + diff <= BUFFER_SIZE); //buffer overflow!
    assert(indexoffset_ + indexdiff <= BUFFER_SIZE); //indexbuffer overflow!

    _InsertBufferData(kPosition,&*position_first,offset_,
		      sizeof(glm::vec3),diff);
    _InsertBufferData(kNormal,&*normal_first,offset_,
		      sizeof(glm::vec3),diff);
    _InsertBufferData(kColor,&*color_first,offset_,
		      sizeof(glm::vec4),diff);
    _InsertIndexBufferData(&*index_first,indexoffset_,
			   sizeof(GLushort),indexdiff);
    auto offset_pair = offset();
    offset_ += diff;// update offset
    indexoffset_ += indexdiff;
    return offset_pair;// Return offset BEFORE update!
  }

  // Watch out this function assumes that Position,Normal and Color Buffer
  // are all of the same length!
  // No boundary checking!
  template <typename PositionIterator,typename NormalIterator,
	    typename ColorIterator,typename IndexIterator>
  void Replace(
      size_t offset,
      size_t indexoffset,
      PositionIterator position_first,
      PositionIterator position_last,
      NormalIterator normal_first,
      ColorIterator color_first,
      IndexIterator index_first,
      IndexIterator index_last) {
    //TODO(mojo): Fix this static_assert block!
    /*		static_assert(is_continuous_iterator<p_iter>::value,
     *                    "Position is not laid out in continuous memory.");
     static_assert(is_continuous_iterator<n_iter>::value,
     *                    "Normal is not laid out in continuous memory.");
     static_assert(is_continuous_iterator<c_iter>::value,
     *                    "Color is not laid out in continuous memory.");
     static_assert(is_continuous_iterator<i_iter>::value,
     *                    "Index is not laid out in continuous memory.");*/

    size_t diff = std::distance(position_first,position_last);
    size_t indexdiff = std::distance(index_first,index_last);

    assert(offset_ + diff <= BUFFER_SIZE); //buffer overflow!
    assert(indexoffset_ + indexdiff <= BUFFER_SIZE); //indexbuffer overflow!

    // maybe throw something instead
    if (offset + diff > offset_) offset_ = offset + diff;
    if (indexoffset + indexdiff > indexoffset_)
      indexoffset_ = indexoffset + indexdiff;

    _InsertBufferData(kPosition,&*position_first,offset,sizeof(glm::vec3),
		      diff);
    _InsertBufferData(kNormal,  &*normal_first,  offset,sizeof(glm::vec3),
		      diff);
    _InsertBufferData(kColor,   &*color_first,   offset,sizeof(glm::vec4),
		      diff);
    _InsertIndexBufferData(&*index_first,indexoffset,sizeof(GLushort),
			   indexdiff);
  }
  // TODO(mojo): Implement this!
  /*	void Replace(
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
	}*/

  std::pair<size_t,size_t> offset() const {
    return std::pair<size_t,size_t>(offset_,indexoffset_);
  }

  void DrawIndexed(GLenum mode = GL_TRIANGLES) {
    glBindVertexArray(va_handle_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer_handle_[kIndex]);
    glDrawElements(mode,static_cast<GLsizei>(indexoffset_),
		   GL_UNSIGNED_SHORT,nullptr);
  }
  void Draw(GLenum mode = GL_TRIANGLES) {
    glBindVertexArray(va_handle_);
    glDrawArrays(mode,0,static_cast<GLsizei>(offset_));
  }

 private:
  void _InitBuffer(Buffer buffer,GLenum target,size_t element_size,
		   GLenum usage_mode) {
    glBindBuffer(target,buffer_handle_[buffer]);
    glBufferData(target,BUFFER_SIZE*element_size,nullptr,usage_mode);
  }

  void _BindVertexPointer(Buffer buffer,GLint element_number,
			  GLenum element_type,GLenum normalized) {
    glBindBuffer(GL_ARRAY_BUFFER,buffer_handle_[buffer]);
    glVertexAttribPointer(buffer,element_number,element_type,
			  normalized,0,0);
  }

  void _InsertBufferData(Buffer buffer,GLvoid const *data_ptr,size_t offset,
			 size_t element_size,size_t set_size){
    glBindBuffer(GL_ARRAY_BUFFER,buffer_handle_[buffer]);
    glBufferSubData(GL_ARRAY_BUFFER,element_size*offset,
		    element_size*set_size,data_ptr);
  }
  void _InsertIndexBufferData(GLvoid const *data_ptr,size_t offset,
			      size_t element_size,size_t set_size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer_handle_[kIndex]);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,element_size*offset,
		    element_size*set_size,data_ptr);
  }
  //TODO(mojo): To be fixed together with static_asserts.
  /*	template <typename T>
	struct is_continuous_iterator:
	std::is_same<
	typename std::iterator_traits<T>::iterator_category,
	std::random_access_iterator_tag
	>
	{};*/

 

  GLuint buffer_handle_[N_BUFFERS];
  GLuint va_handle_;
  size_t offset_;
  size_t indexoffset_;
};

GRAPHICS_END

#undef N_BUFFERS
#undef BUFFER_SIZE

#endif//MFL_GRAPHICS_BUFFERS_INCLUDED
