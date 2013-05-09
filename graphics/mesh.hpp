#ifndef MFL_GRAPHICS_MESH_INCLUDED
#define MFL_GRAPHICS_MESH_INCLUDED

#include <GL/glew.h>

#include "rawmesh.hpp"

GRAPHICS_BEGIN

class SceneNode;
//TODO(mojo): Change fit standards.
struct Mesh {
  Mesh() : raw_(), color_() {}
  Mesh(RawMesh& raw) : raw_(raw), color_() {}
  Mesh(RawMesh& raw,std::vector<glm::vec4> color) : raw_(raw), color_() {
    SetColor(color);
  }
  void SetColor(std::vector<glm::vec4>& color) {
    assert(color.size() == raw_.Size());
    color_ = color;
  }
  void SetColor(glm::vec4 color) {
    color_ = std::vector<glm::vec4>(raw_.Size(),color);
  }
  //===========================
  // members
  //===========================
  RawMesh	raw_;
  std::vector<glm::vec4>	color_;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_MESH_INCLUDED
