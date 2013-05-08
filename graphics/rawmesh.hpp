#ifndef MFL_GRAPHICS_RAW_MESH_INCLUDED
#define MFL_GRAPHICS_RAW_MESH_INCLUDED

#include <cassert>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "common.hpp"

GRAPHICS_BEGIN

struct Mesh;

// mesh without color or texture data
//TODO(mojo): Change to fit standards.
struct RawMesh{
  typedef GLushort	Index;
  typedef glm::vec3	Vec3;
  
  typedef std::vector<Index>	vector_Index;
  typedef std::vector<Vec3>	vector_Vec3;
  
  RawMesh() : position_(), normal_(), index_() {}
  
  void Append(Vec3 const& position,Vec3 const& normal) {
    position_.push_back(position);
    normal_.push_back(normal);
  }
  
  template <typename Iterator>
  void Append(Iterator const position_first,Iterator const position_last,
	      Iterator const normal_first,Iterator const normal_last) {
    position_.insert(position_.end(),position_first,position_last);
    normal_.insert(normal_.end(),normal_first,normal_last);
  }
  
  size_t Size() const {
    assert(position_.size() == normal_.size());
    return position_.size();
  }
  size_t IndexSize() const {
    return index_.size();
  }

  template <typename Iterator>
  void IndexAppend(Iterator const first,Iterator const last) {
    assert(position_.size() == normal_.size());
    index_.insert(index_.end(),first,last);
  }
  //===========================
  // members
  //===========================
  vector_Vec3 position_;
  vector_Vec3 normal_;
  vector_Index index_;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_RAW_MESH_INCLUDED
