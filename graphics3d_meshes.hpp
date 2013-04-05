#ifndef MFL_GRAPHICS3D_SHAPES_INCLUDED
#define MFL_GRAPHICS3D_SHAPES_INCLUDED

#include <vector>
#include "MFL_LinAlg_Utility.hpp"
#define MFL_CONSTANTS
#include "MFL_math.hpp"

#include <GL\glew.h>


namespace MFL{
namespace Graphics3D{

	// if non index drawing -- indices has zero size
	// could always use GL_TRIANGLES and indexed drawing
	template <class T,class IdxType>
	struct Mesh_t{
		std::vector<LinAlg::Vector<T,3>> positions;
		std::vector<LinAlg::Vector<T,3>> normals;
		std::vector<IdxType>			 indices;
		int								 drawtype;

		static const int indextype() {
			return std::is_same<IdxType,GLubyte>::value  ? GL_UNSIGNED_BYTE  :
				   std::is_same<IdxType,GLushort>::value ? GL_UNSIGNED_SHORT :
				   std::is_same<IdxType,GLuint>::value	 ? GL_UNSIGNED_INT   : 0 ;
		}
		static const int datatype(){
			return	std::is_same<T,GLbyte>::value   ? GL_BYTE			:
					std::is_same<T,GLubyte>::value  ? GL_UNSIGNED_BYTE  :

					std::is_same<T,GLshort>::value  ? GL_SHORT			:
					std::is_same<T,GLushort>::value ? GL_UNSIGNED_SHORT :

					std::is_same<T,GLint>::value	? GL_INT			:
					std::is_same<T,GLuint>::value   ? GL_UNSIGNED_INT   :


					std::is_same<T,GLhalf>::value   ? GL_HALF_FLOAT		:
					std::is_same<T,GLfloat>::value  ? GL_FLOAT			:
					std::is_same<T,GLdouble>::value ? GL_DOUBLE			: 0 ;
		}
	
	};
	/*
	template <typename T>
	Shape_t Cylinder(int n){// n is the number of sides


	}
	template <typename T>
	Shape_t Sphere(int n){

	}

	template <typename T>
	Shape_t Circle(int n){

	}
	template <typename T>
	Shape_t<T> Cone(int n){

	}
	*/
	template <typename T,typename IdxType>
	Mesh_t<T,IdxType> Circle(int triangles ){
		using LinAlg::Vector3;
		if( triangles < 3)
			triangles = 3;
		
		LinAlg::Vector<T,3> normal;

		Mesh_t<T,IdxType> Mesh;
		Mesh.positions.resize(triangles+1);
		Mesh.normals.resize(triangles+1);
		Mesh.indices.resize(triangles*3);
		Mesh.drawtype = GL_TRIANGLES;

		Mesh.positions[0] = Vector3<T>(0,0,0);

		for(int i=1;i<=triangles;++i){
			Mesh.positions[i] = Vector3<T>(cos(i*2*MFL_PI/triangles),sin(i*2*MFL_PI/triangles),0);
			Mesh.normals[i]   = Vector3<T>(0,0,-1);
		}
		for(int i=1;i<=triangles;++i){
			Mesh.indices[i*3-3] = 0;
			Mesh.indices[i*3-2] = i;
			Mesh.indices[i*3-1] = i+1;
		}
		return Mesh;
	}

	template <typename T,typename IdxType>
	const Mesh_t<T,IdxType> Cube(){
		using LinAlg::Vector3;
		LinAlg::Vector<T,3> point[8],facenormal[6];

		point[0] = Vector3<T>( 1, 1, 1);
		point[1] = Vector3<T>(-1, 1, 1);
		point[2] = Vector3<T>( 1,-1, 1);
		point[3] = Vector3<T>(-1,-1, 1);
		point[4] = Vector3<T>( 1, 1,-1);
		point[5] = Vector3<T>(-1, 1,-1);
		point[6] = Vector3<T>( 1,-1,-1);
		point[7] = Vector3<T>(-1,-1,-1);


		facenormal[0] = (TriangleNormal(point[0],point[1],point[2])).normalise();
		facenormal[1] = (TriangleNormal(point[4],point[5],point[0])).normalise();
		facenormal[2] = (TriangleNormal(point[6],point[7],point[4])).normalise();
		facenormal[3] = (TriangleNormal(point[2],point[3],point[6])).normalise();
		facenormal[4] = (TriangleNormal(point[0],point[2],point[4])).normalise();
		facenormal[5] = (TriangleNormal(point[1],point[5],point[3])).normalise();

		Mesh_t<T,IdxType> Mesh;
		Mesh.positions.resize(24);
		Mesh.normals.resize(24);
		Mesh.indices.resize(36);
		Mesh.drawtype = GL_TRIANGLES;


		std::fill(&Mesh.normals[0] ,&Mesh.normals[4]   ,facenormal[0]);
		std::fill(&Mesh.normals[4] ,&Mesh.normals[8]   ,facenormal[1]);
		std::fill(&Mesh.normals[8] ,&Mesh.normals[12]  ,facenormal[2]);
		std::fill(&Mesh.normals[12],&Mesh.normals[16]  ,facenormal[3]);
		std::fill(&Mesh.normals[16],&Mesh.normals[20]  ,facenormal[4]);
		std::fill(&Mesh.normals[20],&Mesh.normals[23]+1,facenormal[5]);

		// back
		Mesh.positions[0] = point[0];
		Mesh.positions[1] = point[1];
		Mesh.positions[2] = point[2];
		Mesh.positions[3] = point[3];

		Mesh.indices[0] = 0;
		Mesh.indices[1] = 1;
		Mesh.indices[2] = 2;
		Mesh.indices[3] = 1;
		Mesh.indices[4] = 2;
		Mesh.indices[5] = 3;
		
		// top
		Mesh.positions[4] = point[0];
		Mesh.positions[5] = point[1];
		Mesh.positions[6] = point[4];
		Mesh.positions[7] = point[5];

		Mesh.indices[6] = 4;
		Mesh.indices[7] = 5;
		Mesh.indices[8] = 6;
		Mesh.indices[9] = 5;
		Mesh.indices[10] = 6;
		Mesh.indices[11] = 7;

		// front
		Mesh.positions[8] = point[4];
		Mesh.positions[9] = point[5];
		Mesh.positions[10] = point[6];
		Mesh.positions[11] = point[7];
		
		Mesh.indices[12] = 8;
		Mesh.indices[13] = 9;
		Mesh.indices[14] = 10;
		Mesh.indices[15] = 9;
		Mesh.indices[16] = 10;
		Mesh.indices[17] = 11;
		
		// bottom
		Mesh.positions[12] = point[2];
		Mesh.positions[13] = point[3];
		Mesh.positions[14] = point[6];
		Mesh.positions[15] = point[7];
	
		Mesh.indices[18] = 12;
		Mesh.indices[19] = 13;
		Mesh.indices[20] = 14;
		Mesh.indices[21] = 13;
		Mesh.indices[22] = 14;
		Mesh.indices[23] = 15;
		
		// left
		Mesh.positions[16] = point[0];
		Mesh.positions[17] = point[2];
		Mesh.positions[18] = point[4];
		Mesh.positions[19] = point[6];

		Mesh.indices[24] = 16;
		Mesh.indices[25] = 17;
		Mesh.indices[26] = 18;
		Mesh.indices[27] = 17;
		Mesh.indices[28] = 18;
		Mesh.indices[29] = 19;

		// right
		Mesh.positions[20] = point[1];
		Mesh.positions[21] = point[3];
		Mesh.positions[22] = point[5];
		Mesh.positions[23] = point[7];
		
		Mesh.indices[30] = 20;
		Mesh.indices[31] = 21;
		Mesh.indices[32] = 22;
		Mesh.indices[33] = 21;
		Mesh.indices[34] = 22;
		Mesh.indices[35] = 23;
		
		return Mesh;
	}

	template <typename T,typename IdxType>
	Mesh_t<T,IdxType> Tetrahedron(){
		using LinAlg::Vector3;
		LinAlg::Vector<T,3> point[4], normal[4];

		point[0] = Vector3<T>( 1, 0,-MFL_SQRT2_INV);
		point[1] = Vector3<T>(-1, 0,-MFL_SQRT2_INV);
		point[2] = Vector3<T>( 0, 1, MFL_SQRT2_INV);
		point[3] = Vector3<T>( 0,-1, MFL_SQRT2_INV);


		normal[0] = (TriangleNormal(point[0],point[1],point[2])).normalised();
		normal[1] = (TriangleNormal(point[1],point[3],point[2])).normalised();
		normal[2] = (TriangleNormal(point[2],point[3],point[0])).normalised();
		normal[3] = (TriangleNormal(point[3],point[1],point[0])).normalised();


		Mesh_t<T,IdxType> Mesh;
		Mesh.positions.resize(12);
		Mesh.normals.resize(12);
		Mesh.indices.resize(12);
		Mesh.drawtype = GL_TRIANGLES;

		Mesh.positions[0] = point[0];
		Mesh.positions[1] = point[1];
		Mesh.positions[2] = point[2];
		Mesh.positions[3] = point[1];
		Mesh.positions[4] = point[2];
		Mesh.positions[5] = point[3];
		Mesh.positions[6] = point[2];
		Mesh.positions[7] = point[3];
		Mesh.positions[8] = point[0];
		Mesh.positions[9]  = point[3];
		Mesh.positions[10] = point[1];
		Mesh.positions[11] = point[0];

		std::fill(&Mesh.normals[0],&Mesh.normals[3],normal[0]);
		std::fill(&Mesh.normals[3],&Mesh.normals[6],facenormal[1]);
		std::fill(&Mesh.normals[6],&Mesh.normals[9],facenormal[2]);
		std::fill(&Mesh.normals[9],&Mesh.normals[12],facenormal[3]);

		Mesh.indices[0] = 0;
		Mesh.indices[1] = 1;
		Mesh.indices[2] = 2;
		Mesh.indices[3] = 3;
		Mesh.indices[4] = 4;
		Mesh.indices[5] = 5;
		Mesh.indices[6] = 6;
		Mesh.indices[7] = 7;
		Mesh.indices[8] = 8;
		Mesh.indices[9] = 9;
		Mesh.indices[10] = 10;
		Mesh.indices[11] = 11;

		return Mesh;
	}

}
}

#endif//MFL_GRAPHICS3D_SHAPES_INCLUDED