#ifndef MFL_GRAPHICS_RAW_MESH_INCLUDED
#define MFL_GRAPHICS_RAW_MESH_INCLUDED

#include "MFL_Graphics_Common.hpp"

#include <vector>
#include <algorithm>
#include <xfunctional>

#include <glm\glm.hpp>
#include <GL\glew.h>

GRAPHICS_BEGIN

struct mesh;

struct raw_mesh{ // mesh without color or texture data
public:
	typedef GLushort						idx_t;
	typedef glm::vec3						value_t;

	typedef std::vector<value_t>			value_vec_t;
	typedef std::vector<idx_t>				idx_vec_t;

//private:
	value_vec_t	_position;
	value_vec_t _normal;
	idx_vec_t	_index;

	friend mesh;

public:
	raw_mesh()
		:_position(),_normal(),_index(){}
	raw_mesh(raw_mesh const&  raw)
		:_position(raw._position),
		_normal(raw._normal),
		_index(raw._index){}
	explicit raw_mesh(raw_mesh&& raw)
		:_position	(std::forward<raw_mesh::value_vec_t>(raw._position)	),
		_normal		(std::forward<raw_mesh::value_vec_t>(raw._normal)	),
		_index		(std::forward<raw_mesh::idx_vec_t>	(raw._index)	){}

	void append(value_t&& p,value_t&& n){
		_position.push_back(std::forward<value_t>(p));
		_normal.push_back(std::forward<value_t>(n));
	}
	void append(const value_t& p,const value_t& n){
		_position.push_back(p);
		_normal.push_back(n);
	}

	template <typename iter_t>
	void append(const iter_t pfirst,const iter_t plast,
		const iter_t nfirst,const iter_t nlast){
		_position.insert(_position.end(),pfirst,plast);
		_normal.insert(_normal.end(),nfirst,nlast);
	}

	size_t size() const{
		return _position.size();
	}
	size_t index_size() const{
		return _index.size();
	}

	template <typename iter_t>
	void index_append(const iter_t first,const iter_t last){

		assert(_normal.size() == _position.size());
		_index.insert(_index.end(),first,last);
	}

};

GRAPHICS_END

#endif//MFL_GRAPHICS_RAW_MESH_INCLUDED