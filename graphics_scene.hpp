#ifndef MFL_GRAPHICS_SCENE_INCLUDED
#define MFL_GRAPHICS_SCENE_INCLUDED

#include "MFL_Graphics_SceneNode.hpp"

GRAPHICS_BEGIN

// Discussion: it might be prefereable to use pointers or references to the SceneNodes instead!
class Scene{

public:
	Scene():_Node(),_Buffers(){}
	void addSceneNode(SceneNode& node){
		_Node.push_back(node); 
	}
	// must be called before drawing
	void generateBuffers(){
		_Buffers.Generate();
	}

	void bind(){
		for( auto& n : _Node )
			n.recursivebind(_Buffers);
	}

	void transform(glm::mat4& tf){
		for( auto& n : _Node )
			n.transform(tf,_Buffers);
	}
	void draw(GLenum mode=GL_TRIANGLES){
		_Buffers.Draw(mode);
	}
	void drawIndexed(GLenum mode = GL_TRIANGLES){
		_Buffers.DrawIndexed(mode);
	}
private:
	std::vector<SceneNode>		_Node;
	Buffers						_Buffers;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SCENE_INCLUDED