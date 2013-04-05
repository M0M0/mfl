#ifndef MFL_GRAPHICS3D_SCENE_INCLUDED
#define MFL_GRAPHICS3D_SCENE_INCLUDED

#include <vector>
#include <algorithm>
#include <xfunctional>
#include "MFL_LinAlg.hpp"
#include "MFL_Graphics3D_Meshes.hpp"

#include "MFL_GL_Buffer.hpp"
#include "MFL_GL_VertexArray.hpp"

namespace MFL{
namespace Graphics3D{
	template <typename T,typename IdxType>
	struct Object{
		typedef LinAlg::Vector<T,3> vec3_t;
		typedef LinAlg::Vector<T,4> vec4_t;

		typedef LinAlg::Matrix<T,3> mat3_t;
		typedef LinAlg::Matrix<T,4> mat4_t;

		template <typename _Ty1,typename _Ty2>
		struct mult{
			auto operator()(_Ty1 y1,_Ty2 y2) -> decltype(vec3_t(y1*vec4_t(y2))){
				return vec3_t(y1*vec4_t(y2));
			}
		};

		std::vector<vec3_t> _position;
		std::vector<vec3_t> _normal;
		std::vector<vec4_t> _color;
	
		std::vector<IdxType>				  _index;

		Object():_position(),_normal(),_color(),_index(){}
		Object(Object& obj):_position(obj._position),_normal(obj._normal),_color(obj._color),_index(obj._index){}
		Object(Mesh_t<T,IdxType>& mesh,std::vector<vec4_t> color)
			:_position(mesh.positions),_normal(mesh.normals),_color(color),_index(mesh.indices) {}

		void AddObject(Object& obj){
			_index.insert(_index.end(),obj._index.begin(),obj._index.end());
			std::transform(	_index.end()-obj._index.size(),
							_index.end(),
							_index.end()-obj._index.size(),
							std::bind2nd(std::plus<IdxType>(), static_cast<unsigned int>(_position.size())));

			_position.insert(_position.end(),obj._position.begin(),obj._position.end());
			_normal.insert(_normal.end(),obj._normal.begin(),obj._normal.end());
			_color.insert(_color.end(),obj._color.begin(),obj._color.end());
		}

		void transform(mat4_t mat){
			std::for_each(_position.begin(),_position.end(),std::bind1st(mult(),mat));
			std::for_each(_normal.begin(),_normal.end(),std::bind1st(mult(),mat));
		}
		void transform(mat3_t rotation,vec3_t translation=LinAlg::Vector3<T>(0,0,0)){
			for(auto iter = _position.begin();iter!=_position.end();++iter)
				*iter = rotation* (*iter) + translation;
			for(auto iter = _normal.begin();iter != _normal.end();++iter)
				*iter = rotation* (*iter);
		}

	};

	// only for static objects!
	template <typename T,typename IdxType>
	class Scene{
	protected:
		Object<T,IdxType>    _obj;

		MFL::GL::Buffer      _vertex, _index;

		MFL::GL::VertexArray _VAO;
	public:
		Scene(){}
		~Scene(){}

		void Add(Object<T,IdxType>& obj) {
			_obj.AddObject(obj);
		}
		void GenerateBuffersAndVA(){
			_index.Construct(GL_ELEMENT_ARRAY_BUFFER,sizeof(IdxType)*_obj._index.size(),GL_STATIC_DRAW,&(_obj._index[0]));

			//might construct unexpected errors look out!
			_vertex.Construct(GL_ARRAY_BUFFER,sizeof(T)*(3+3+4)*_obj._position.size(),GL_STATIC_DRAW);

			_vertex.SubData(0									 ,sizeof(T)*3*_obj._position.size(),&(_obj._position[0]));
			_vertex.SubData(sizeof(T)*3*_obj._position.size()    ,sizeof(T)*3*_obj._normal.size()  ,&(_obj._normal[0]));
			_vertex.SubData(sizeof(T)*(3+3)*_obj._position.size(),sizeof(T)*4*_obj._color.size()   ,&(_obj._color[0]));

			_VAO.Gen();
			_VAO.Bind();

			_VAO.EnableAttribute(GL::Vertex);
			_VAO.EnableAttribute(GL::Normal);
			_VAO.EnableAttribute(GL::Color);

			_vertex.Bind();

			_VAO.AttributePointer(GL::Vertex,3,datatype(),GL_FALSE,0,0);
			_VAO.AttributePointer(GL::Normal,3,datatype(),GL_TRUE ,0,(GLvoid*)(sizeof(T)*3*_obj._position.size()));
			_VAO.AttributePointer(GL::Color, 4,datatype(),GL_TRUE ,0,(GLvoid*)(sizeof(T)*(3+3)*_obj._position.size()));

			_VAO.Release();
			_vertex.Release();
			_index.Release();
		}
		void draw(){
			_VAO.Bind();
			_index.Bind();
			glDrawElements(GL_TRIANGLES,static_cast<GLsizei>(_obj._index.size()),indextype(),0);
			_index.Release();
			_VAO.Release();
		}

		static const int indextype() {
			return  std::is_same<IdxType,GLubyte> ::value ? GL_UNSIGNED_BYTE  :
					std::is_same<IdxType,GLushort>::value ? GL_UNSIGNED_SHORT :
					std::is_same<IdxType,GLuint>  ::value ? GL_UNSIGNED_INT   : 0 ;
		}
		static const int datatype(){
			return	std::is_same<T,GLbyte>	::value ? GL_BYTE			:
					std::is_same<T,GLubyte>	::value ? GL_UNSIGNED_BYTE  :
					std::is_same<T,GLshort>	::value ? GL_SHORT			:
					std::is_same<T,GLushort>::value ? GL_UNSIGNED_SHORT :
					std::is_same<T,GLint>	::value	? GL_INT			:
					std::is_same<T,GLuint>	::value ? GL_UNSIGNED_INT   :
					std::is_same<T,GLhalf>	::value ? GL_HALF_FLOAT		:
					std::is_same<T,GLfloat>	::value ? GL_FLOAT			:
					std::is_same<T,GLdouble>::value ? GL_DOUBLE			: 0 ;
		}


	};
	

}
}

#endif//MFL_GRAPHICS3D_SCENE_INCLUDED