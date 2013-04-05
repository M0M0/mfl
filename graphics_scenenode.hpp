#ifndef MFL_GRAPHICS_SCENENODE_INCLUDED
#define MFL_GRAPHICS_SCENENODE_INCLUDED

#include "MFL_Graphics_Mesh.hpp"
#include "MFL_Graphics_Buffers.hpp"

#include <algorithm>
#include <vector>
#include <memory>

GRAPHICS_BEGIN

class SceneNode{
	typedef std::shared_ptr<const mesh>	meshPtr;
public:
	SceneNode()
		:_tf(),_subNode(),_meshptr(nullptr),
		 _bound(false),_moved(true){}

	//=========================================================
	// adds a child node that will be transformed with the node
	//=========================================================
	void addSubNode(SceneNode node){
		_subNode.push_back(node);
	}
	//=========================================================
	// sets a shared pointer to mesh
	//=========================================================
	void bindMesh(meshPtr meshptr){
		_meshptr = meshptr;
	}

	//=========================================================
	// transforms Node without interaction with the buffer
	//=========================================================
	void transform(glm::mat4& tf){
		// transform matrix
		_tf = tf*_tf;
		_moved = true;

		// call childs
		for( auto& n : _subNode)
			n.transform(tf);  
	}

	//=========================================================
	// transforms Node and commits mesh to the buffer
	//=========================================================
	void transform(glm::mat4& tf,Buffers& buffers)
	{
		transform(tf);
		recursivebind(buffers);
	}

	//=========================================================
	// commits mesh and all submeshes to buffer
	//=========================================================
	void recursivebind(Buffers& buffers){
		_bindToBuffer(buffers);
		// call childs
		for( auto& n : _subNode)
			n.recursivebind(buffers);
	}


private:
	//=========================================================
	// Private methods
	//=========================================================
	void _bindToBuffer(Buffers& buffers){
		//only write into the buffer if moved
		if( !_moved && _bound )
			return; 

		std::vector<glm::vec3> tp;
		std::vector<glm::vec3> tn;
		std::vector<GLushort>  ti;

		_transformMesh(_tf,buffers,tp,tn,ti);

		// add to buffers
		if(!_bound){
			_offset = buffers.append(tp.begin(),tp.end(),
				tn.begin(),_meshptr->_color.begin(),
				ti.begin(),ti.end());
			_bound = true;
		}
		else
			buffers.replace(_offset.first,_offset.second,
				tp.begin(),tp.end(),
				tn.begin(),_meshptr->_color.begin(),
				ti.begin(),ti.end());
		
		_moved = false;
	}

	void _transformMesh(glm::mat4& tf,
		Buffers& buffers,
		std::vector<glm::vec3>& p,
		std::vector<glm::vec3>& n,
		std::vector<GLushort> & i){

		glm::mat3 tmat(tf);
		p = _meshptr->_raw._position;
		n = _meshptr->_raw._normal;
		i = _meshptr->_raw._index;

		// transform positions
		for( auto& p : p ){
			glm::vec4 t = _tf*glm::vec4(p,1);
			p = glm::vec3(t)/t.w;
		}

		// transform normals
		for( auto& n : n )
			n = tmat*n;

		// transform indices
		if(!_bound){
			auto offset = buffers.getoffset(); 
			for( auto& i : i )
				i += offset.first;
		}
		else
			for( auto& i : i )
				i += _offset.first;
	}


private:
	//=========================================================
	// Members
	//=========================================================
	glm::mat4					_tf;
	std::vector<SceneNode>		_subNode;
	meshPtr						_meshptr;

	bool						_moved;
	bool						_bound;
	std::pair<size_t,size_t>	_offset;
};

GRAPHICS_END

#endif//MFL_GRAPHICS_SCENENODE_INCLUDED