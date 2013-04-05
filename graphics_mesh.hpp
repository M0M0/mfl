#ifndef MFL_GRAPHICS_MESH_INCLUDED
#define MFL_GRAPHICS_MESH_INCLUDED

#include "MFL_Graphics_raw_mesh.hpp"

#include <GL\glew.h>

GRAPHICS_BEGIN

class SceneNode;

struct mesh{
//private:
	raw_mesh				_raw;
	std::vector<glm::vec4>	_color;

public:
	mesh():_raw(),_color(){}
	mesh(raw_mesh& raw)
		:_raw(raw),_color(){}
	mesh(raw_mesh& raw,std::vector<glm::vec4>& color)
		:_raw(raw),_color(){
		setcolor(color);
	}

	void setcolor(std::vector<glm::vec4>& color){
		assert(color.size() == _raw.size());
		_color = color;
	}
};

GRAPHICS_END

#endif//MFL_GRAPHICS_MESH_INCLUDED