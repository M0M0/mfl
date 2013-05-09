#ifndef MFL_GRAPHICS_SCENENODE_INCLUDED
#define MFL_GRAPHICS_SCENENODE_INCLUDED

#include <algorithm>
#include <vector>
#include <memory>

#include "mesh.hpp"
#include "buffers.hpp"

GRAPHICS_BEGIN

class SceneNode {
  typedef std::shared_ptr<Mesh const>	MeshPtr;
 public:
  SceneNode() : tf_(),subnode_(),meshptr_(nullptr),bound_(false),moved_(true) {
  }
  // adds a child node that will be transformed with the node
  void AddSubNode(SceneNode node) {
    subnode_.push_back(node);
  }
  // sets a shared pointer to mesh
  void BindMesh(MeshPtr meshptr) {
    meshptr_ = meshptr;
  }
  // transforms Node without interaction with the buffer
  void TransformNoUpload(glm::mat4 const& transform) {
    tf_ = transform*tf_;// transform matrix
    moved_ = true;
    for (SceneNode& n : subnode_) n.TransformNoUpload(transform); //call childs
  }
  // transforms Node and commits mesh to the buffer
  void Transform(glm::mat4 const& transform,Buffers& buffers) {
    TransformNoUpload(transform);
    RecursiveBind(buffers);
  }
  // commits mesh and all submeshes to buffer
  void RecursiveBind(Buffers& buffers) {
    _BindToBuffer(buffers);
    for (SceneNode& n : subnode_) n.RecursiveBind(buffers); // call childs
  }
private:
  // Private methods
  void _BindToBuffer(Buffers& buffers){
    //only write into the buffer if moved
    if (!moved_ && bound_) return;
    
    std::vector<glm::vec3> tp;
    std::vector<glm::vec3> tn;
    std::vector<GLushort>  ti;
    
    _TransformMesh(tf_,buffers,tp,tn,ti);
    
    // add to buffers
    if (!bound_) {
      offset_ = buffers.Append(tp.begin(),tp.end(),tn.begin(),
			       meshptr_->color_.begin(),ti.begin(),ti.end());
      bound_ = true;
    } else {
      buffers.Replace(offset_.first,offset_.second,tp.begin(),tp.end(),
		      tn.begin(),meshptr_->color_.begin(),ti.begin(),ti.end());
    }
    moved_ = false;
  }
  void _TransformMesh(
      glm::mat4& tf,
      Buffers& buffers,
      std::vector<glm::vec3>& position,
      std::vector<glm::vec3>& normal,
      std::vector<GLushort> & index) {
    glm::mat3 tmat(tf);
    position = meshptr_->raw_.position_;
    normal   = meshptr_->raw_.normal_;
    index    = meshptr_->raw_.index_;
    
    // transform positions
    for (auto& p : position) {
      glm::vec4 t = tf_*glm::vec4(p,1);
      p = glm::vec3(t)/t.w;
    }
    for (auto& n : normal) n = tmat*n; // transform normals

    if (!bound_) { // transform indices
      auto offset = buffers.offset(); 
      for (auto& i : index) i += offset.first;  
    } else {
      for (auto& i : index) i += offset_.first;
    }
  }

  glm::mat4 tf_;
  std::vector<SceneNode> subnode_;
  MeshPtr meshptr_;
  bool moved_;
  bool bound_;
  std::pair<size_t,size_t> offset_;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SCENENODE_INCLUDED
