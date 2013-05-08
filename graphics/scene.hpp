#ifndef MFL_GRAPHICS_SCENE_INCLUDED
#define MFL_GRAPHICS_SCENE_INCLUDED

#include "common.hpp"
#include "scenenode.hpp"

GRAPHICS_BEGIN

// It might be prefereable to use pointers
//  or references to the SceneNodes instead!
class Scene{
public:
  Scene() : node_(), buffers_() {}
  void AddSceneNode(SceneNode& node){
    node_.push_back(node);
  }
  // must be called before drawing
  void GenerateBuffers() {
    buffers_.Generate();
  }

  void Bind() {
    for (auto& n : node_) n.RecursiveBind(buffers_);
  }

  void Transform(glm::mat4& tf) {
    for (auto& n : node_) n.Transform(tf,buffers_);
  }
  void Draw(GLenum mode = GL_TRIANGLES) {
    buffers_.Draw(mode);
  }
  void DrawIndexed(GLenum mode = GL_TRIANGLES) {
    buffers_.DrawIndexed(mode);
  }
private:
  std::vector<SceneNode> node_;
  Buffers buffers_;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SCENE_INCLUDED
