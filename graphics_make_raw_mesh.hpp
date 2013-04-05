//=================================================================================
// a collection of simple meshes 
// TODO: set all indices to ccw triangles (GL_CULL_FACE)
//  -> fixed for cube
//  -> fixed for cylinder
//  -> fixed for tetrahedron
//=================================================================================
#ifndef MFL_GRAPHICS_MAKE_RAW_MESH_INCLUDED
#define MFL_GRAPHICS_MAKE_RAW_MESH_INCLUDED

#include "MFL_Graphics_raw_mesh.hpp"
#include "MFL_Graphics_Utilities.hpp"

#include <glm\gtc\constants.hpp>
#include <array>

GRAPHICS_BEGIN

const raw_mesh makeCube(float x=1,float y=1,float z=1){
	raw_mesh Cube;

	const std::array<raw_mesh::idx_t,36> index_data = //just the raw index data
	{ 
	   0, 1, 2,	 3, 2, 1, //back
	   6, 5, 4,	 5, 6, 7, //top
	  10, 9, 8,	 9,10,11, //front

	  12,13,14, 15,14,13, //bottom
	  16,17,18, 19,18,17, //left
	  22,21,20, 21,22,23  //right
	};
	Cube.index_append(index_data.cbegin(),index_data.cend());

	glm::vec3 point[8], normal[6];

	point[0] = glm::vec3( x, y, z);
	point[1] = glm::vec3(-x, y, z);
	point[2] = glm::vec3( x,-y, z);
	point[3] = glm::vec3(-x,-y, z);
	point[4] = glm::vec3( x, y,-z);
	point[5] = glm::vec3(-x, y,-z);
	point[6] = glm::vec3( x,-y,-z);
	point[7] = glm::vec3(-x,-y,-z);

	normal[0] = trianglenormal(point[0],point[1],point[2]);
	normal[1] = trianglenormal(point[4],point[5],point[0]);
	normal[2] = trianglenormal(point[6],point[7],point[4]);
	normal[3] = trianglenormal(point[2],point[3],point[6]);
	normal[4] = trianglenormal(point[0],point[2],point[4]);
	normal[5] = trianglenormal(point[1],point[5],point[3]);

	Cube.append(point[0],normal[0]);
	Cube.append(point[1],normal[0]);
	Cube.append(point[2],normal[0]);
	Cube.append(point[3],normal[0]);

	Cube.append(point[0],normal[1]);
	Cube.append(point[1],normal[1]);
	Cube.append(point[4],normal[1]);
	Cube.append(point[5],normal[1]);

	Cube.append(point[4],normal[2]);
	Cube.append(point[5],normal[2]);
	Cube.append(point[6],normal[2]);
	Cube.append(point[7],normal[2]);

	Cube.append(point[2],normal[3]);
	Cube.append(point[3],normal[3]);
	Cube.append(point[6],normal[3]);
	Cube.append(point[7],normal[3]);

	Cube.append(point[0],normal[4]);
	Cube.append(point[2],normal[4]);
	Cube.append(point[4],normal[4]);
	Cube.append(point[6],normal[4]);

	Cube.append(point[1],normal[5]);
	Cube.append(point[3],normal[5]);
	Cube.append(point[5],normal[5]);
	Cube.append(point[7],normal[5]);

	return Cube;
}

const raw_mesh makeTetrahedron(){
	using glm::root_two;
	raw_mesh Tetra;

	const std::array<raw_mesh::idx_t,12> index_data = //just the raw index data
	{
		0, 1, 2, // top     - front
		5, 4, 3, // back    - right
		6, 7, 8, // back    - left
		11,10,9  // bottom  - front
	};
	Tetra.index_append(index_data.cbegin(),index_data.cend());

	glm::vec3 point[4], normal[4];

	point[0] = glm::vec3( 1, 0,-1/root_two<float>());
	point[1] = glm::vec3(-1, 0,-1/root_two<float>());
	point[2] = glm::vec3( 0, 1, 1/root_two<float>());
	point[3] = glm::vec3( 0,-1, 1/root_two<float>());

	normal[0] = trianglenormal(point[0],point[1],point[2]);
	normal[1] = trianglenormal(point[1],point[3],point[2]);
	normal[2] = trianglenormal(point[2],point[3],point[0]);
	normal[3] = trianglenormal(point[3],point[1],point[0]);

	Tetra.append(point[0],normal[0]);
	Tetra.append(point[1],normal[0]);	
	Tetra.append(point[2],normal[0]);

	Tetra.append(point[1],normal[1]);
	Tetra.append(point[2],normal[1]);	
	Tetra.append(point[3],normal[1]);

	Tetra.append(point[2],normal[2]);
	Tetra.append(point[3],normal[2]);	
	Tetra.append(point[0],normal[2]);

	Tetra.append(point[3],normal[3]);
	Tetra.append(point[0],normal[3]);
	Tetra.append(point[1],normal[3]);

	return Tetra;
}

// still some problems with normals when mesh is called with arguments less than (x,40,40)
const raw_mesh makeSphere(float r=1,int nsteps_theta=3,int nsteps_phi=3){
	using glm::sin;
	using glm::cos;
	using glm::vec3;
	using glm::pi;

	vec3 normal;

	if(nsteps_theta < 3)
		nsteps_theta = 3;
	if(nsteps_phi < 3)
		nsteps_phi = 3;

	float dtheta = pi<float>()*1/(nsteps_theta-1);
	float dphi   = pi<float>()*2/nsteps_phi;

	raw_mesh Sphere;

	normal = vec3(0,0,1);         // top at index 0
	Sphere.append(r*normal,normal);

	for(float theta=dtheta; theta<pi<float>(); theta+=dtheta)	
		for(float phi=0; phi<pi<float>()*2; phi+=dphi){
			normal = vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			Sphere.append(r*normal,normal);
		}

	normal = vec3(0,0,-1); // bottom at last index
	Sphere.append(r*normal,normal);

	int npoints = Sphere.size();

	raw_mesh::idx_vec_t indexlist;

	for(int i=1; i<(nsteps_phi+1); ++i){ // for the uppermost loop
		indexlist.push_back( 0 );
		indexlist.push_back( i );
		if( i+1 == nsteps_phi+1 )
			indexlist.push_back( 1 );
		else
			indexlist.push_back( i+1 );
	}

	if(nsteps_theta>3)
		for(int i=nsteps_phi+1; i<npoints-1; ++i){
			indexlist.push_back( i );
			indexlist.push_back( i-nsteps_phi );
			if( i%nsteps_phi == 0 )
				indexlist.push_back( i+1-2*nsteps_phi );
			else
				indexlist.push_back( i+1-nsteps_phi );

			indexlist.push_back( i );
			indexlist.push_back( i+1-nsteps_phi );
			if( i%nsteps_phi == 0 )
				indexlist.push_back( i+1-2*nsteps_phi );	
			else
				indexlist.push_back( i+1 );
		}

	for(int i=npoints-(nsteps_phi+1); i<npoints-1; ++i){ // for the lowerest loop
		indexlist.push_back( npoints-1 );
		indexlist.push_back( i );
		if( i+1 == npoints-1 )
			indexlist.push_back( npoints-(nsteps_phi+1) );
		else
			indexlist.push_back( i+1 );
	}

	Sphere.index_append(indexlist.begin(),indexlist.end());

	return Sphere;
}

const raw_mesh makeCylinder(float r=1,float z=1,int nsteps_phi=3){
	using glm::vec2;
	using glm::vec3;
	using glm::pi;

	float dphi = pi<float>() * 2/nsteps_phi;

	raw_mesh Cylinder;
	Cylinder.append(vec3(0,0,z),vec3(0,0, 1)); // top

	vec2 xy;
	for( float phi=0; phi < 2*pi<float>(); phi+=dphi ){
		xy = vec2(glm::cos(phi),glm::sin(phi));
		Cylinder.append(vec3(r*xy, z),vec3(0,0, 1));
		Cylinder.append(vec3(r*xy, z),vec3(xy,  0));
		Cylinder.append(vec3(r*xy,-z),vec3(xy,  0));
		Cylinder.append(vec3(r*xy,-z),vec3(0,0,-1));
	}
	Cylinder.append(vec3(0,0,-z),vec3(0,0,-1));	// bottom

	int npoints = Cylinder.size();

	raw_mesh::idx_vec_t indexlist;
	auto top    = 0;
	auto bottom = npoints - 1;

	for( int i=1; i < npoints-5; i+=4 ){
		indexlist.push_back( top );
		indexlist.push_back( i   );
		indexlist.push_back( i+4 );

		indexlist.push_back( i+1 );
		indexlist.push_back( i+2 );
		indexlist.push_back( i+5 );

		indexlist.push_back( i+6 );
		indexlist.push_back( i+5 );
		indexlist.push_back( i+2 );

		indexlist.push_back( bottom );
		indexlist.push_back( i+7 );
		indexlist.push_back( i+3 );
	}
	// last loop back to first indices
	indexlist.push_back( top );
	indexlist.push_back( npoints-5 );
	indexlist.push_back( 1 );

	indexlist.push_back( npoints-4 );
	indexlist.push_back( npoints-3 );
	indexlist.push_back( 2 );

	indexlist.push_back( 3 );
	indexlist.push_back( 2 );
	indexlist.push_back( npoints-3 );

	indexlist.push_back( bottom );
	indexlist.push_back( 4 );
	indexlist.push_back( npoints-2 );

	Cylinder.index_append(indexlist.begin(),indexlist.end());	

	return Cylinder;
}

GRAPHICS_END


#endif//MFL_GRAPHICS_MAKE_RAW_MESH_INCLUDED