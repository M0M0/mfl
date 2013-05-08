#ifndef MFL_GL_MESH_INCLUDED
#define MFL_GL_MESH_INCLUDED

#include "MFL_LinAlg_Vector.hpp"
#include "MFL_GL_Buffer.hpp"
#include "MFL_GL_VertexArray.hpp"

namespace MFL{
namespace GL{
	class Mesh{
	protected:
		GLenum _primitiveType;
		bool _done;
		
		MFL::LinAlg::Vector4f *_ptrVertices, *_ptrColors;
		MFL::LinAlg::Vector3f *_ptrNormals;
		MFL::LinAlg::Vector2f **_ptrTexCoords;

		Buffer  _bufferVertex;
		Buffer  _bufferNormal;
		Buffer  _bufferColor;
		Buffer* _bufferTexCoord;

		VertexArray _vertexArray;

		int _nTextures;
		int _nVertices;

	public:

		Mesh():_done(false),_ptrVertices(nullptr), _ptrNormals(nullptr),
			_ptrColors(nullptr), _ptrTexCoords(nullptr),
			_nTextures(0), _nVertices(0){
		}
		virtual ~Mesh(){
			delete[] _ptrVertices;
			delete[] _ptrNormals;
			delete[] _ptrColors;
			
			delete[] _bufferTexCoord;

			for(int i=0;i<_nTextures;++i)
				delete[] _ptrTexCoords[i];
			delete[] _ptrTexCoords;
		}

		void Begin(GLenum primitive,GLuint nVerts,GLuint nTex=0){
			_primitiveType = primitive;
			_nVertices     = nVerts;
			_nTextures     = nTex;

			if( _nTextures != 0 ){
				_bufferTexCoord = new Buffer[_nTextures];
				_ptrTexCoords = new MFL::LinAlg::Vector2f*[_nTextures];

				for(int i=0;i<_nTextures;++i)
					_ptrTexCoords[i] = nullptr;
			}
			_vertexArray.Gen();
		}
		void End(){
			if(_ptrVertices!=nullptr){
				_bufferVertex.Unmap();
				_ptrVertices=nullptr;
			}
			if(_ptrNormals!=nullptr){
				_bufferNormal.Unmap();
				_ptrNormals=nullptr;
			}
			if(_ptrColors!=nullptr){
				_bufferColor.Unmap();
				_ptrColors=nullptr;
			}
			for(int i=0;i<_nTextures;++i)
				if(_ptrTexCoords!=nullptr){
					_bufferTexCoord[i].Unmap();
					_ptrTexCoords=nullptr;
				}


			if(_bufferVertex()!=0){
				_bufferVertex.Bind();
				_vertexArray.AttributePointer(Vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}
			if(_bufferNormal()!=0){
				_bufferNormal.Bind();
				_vertexArray.AttributePointer(Normal, 4, GL_FLOAT, GL_FALSE, 0, 0);
			}
			if(_bufferColor()!=0){
				_bufferColor.Bind();
				_vertexArray.AttributePointer(Color, 3, GL_FLOAT, GL_FALSE, 0, 0);
			}

			for(int i=0;i<_nTextures;++i)
				if(_bufferTexCoord[i]()!=0){
					_bufferTexCoord[i].Bind();
					_vertexArray.AttributePointer((VertexAttribute)(Texture0 + i), 2, GL_FLOAT, GL_FALSE, 0, 0);
				}
			_done = true;
			_vertexArray.Release();
		}
		void Reset(){
			_done = false;
			_nVertices = 0;
		}

		virtual void Draw(){
			if(!_done)
				return;

			_vertexArray.Bind();		
			glDrawArrays(_primitiveType,0,_nVertices);
			_vertexArray.Release();
		}

		void CopyVertices(MFL::LinAlg::Vector4f *v){
			if(_bufferVertex()==0)
				_bufferVertex.Construct(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,sizeof(LinAlg::Vector4f)*_nVertices,v);
			else{
				_bufferVertex.SubData(0,sizeof(LinAlg::Vector4f)*_nVertices,v);
				_ptrVertices = nullptr;
			}
		}
		void CopyVertices(float                 *v){
			CopyVertices( (MFL::LinAlg::Vector4f*)v);
		}

		void CopyNormals(MFL::LinAlg::Vector3f *n){
			if(_bufferNormal()==0)
				_bufferNormal.Construct(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,sizeof(LinAlg::Vector3f)*_nVertices,n);
			else{
				_bufferNormal.SubData(0,sizeof(LinAlg::Vector3f)*_nVertices,n);
				_ptrNormals = nullptr;
			}

		}
		void CopyNormals(float				   *n){
			CopyNormals((MFL::LinAlg::Vector3f*)n);
		}

		void CopyColors(MFL::LinAlg::Vector4f *c){
			if(_bufferColor()==0)
				_bufferColor.Construct(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,sizeof(LinAlg::Vector4f)*_nVertices,c);
			else{
				_bufferColor.SubData(0,sizeof(LinAlg::Vector4f)*_nVertices,c);
				_ptrColors = nullptr;
			}

		}
		void CopyColors(float				  *c){
			CopyColors((MFL::LinAlg::Vector4f*)c);
		}

		void CopyTextureCoords(MFL::LinAlg::Vector2f *t, GLuint iTex){
			if(_bufferTexCoord[iTex]()==0)
				_bufferTexCoord[iTex].Construct(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW,sizeof(LinAlg::Vector2f)*_nVertices,t);
			else{
				_bufferTexCoord[iTex].SubData(0,sizeof(LinAlg::Vector2f)*_nVertices,t);
				_ptrTexCoords[iTex] = nullptr;
			}
		}
		void CopyTextureCoords(float				 *t, GLuint iTex){
			CopyTextureCoords((MFL::LinAlg::Vector2f*)t,iTex);
		}

	};
}
}

#endif//MFL_GL_MESH_INCLUDED