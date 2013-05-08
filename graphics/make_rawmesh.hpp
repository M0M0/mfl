//==============================================================================
// a collection of simple meshes 
// TODO: set all indices to ccw triangles (GL_CULL_FACE)
//  -> fixed for cube
//  -> fixed for cylinder
//  -> fixed for tetrahedron
//==============================================================================
#ifndef MFL_GRAPHICS_MAKE_RAW_MESH_INCLUDED
#define MFL_GRAPHICS_MAKE_RAW_MESH_INCLUDED

#include <glm/gtc/constants.hpp>

#include "graphics_rawmesh.hpp"
#include "graphics_utilities.hpp"

GRAPHICS_BEGIN

RawMesh makeCube(float x=1,float y=1,float z=1) {
  const unsigned int index_size = 36;
  const RawMesh::Index index_data[index_size] = //just the raw index data
  { 
     0, 1, 2,  3, 2, 1,  //back
     6, 5, 4,  5, 6, 7,  //top
    10, 9, 8,  9,10,11, //front
    12,13,14, 15,14,13, //bottom
    16,17,18, 19,18,17, //left
    22,21,20, 21,22,23  //right
  };
  RawMesh Cube;
  Cube.IndexAppend(index_data,index_data+index_size);
  
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
  
  auto AppendSide =
    [&](int p0,int p1,int p2,int p3,int n) {
      Cube.Append(point[p0],normal[n]);
      Cube.Append(point[p1],normal[n]);
      Cube.Append(point[p2],normal[n]);
      Cube.Append(point[p3],normal[n]);
    };
        
  AppendSide(0,1,2,3,0);
  AppendSide(0,1,4,5,1);
  AppendSide(4,5,6,7,2);
  AppendSide(2,3,6,7,3);
  AppendSide(0,2,4,6,4);
  AppendSide(1,3,5,7,5);

  return Cube;
}

RawMesh makeTetrahedron(){
  using glm::root_two;
  const unsigned int index_size = 12;
  const RawMesh::Index index_data[index_size] =//just the raw index data
  {
    0, 1, 2, // top     - front
    5, 4, 3, // back    - right
    6, 7, 8, // back    - left
    11,10,9  // bottom  - front
  };
  RawMesh Tetra;
  Tetra.IndexAppend(index_data,index_data+index_size);
  
  glm::vec3 point[4], normal[4];
  point[0] = glm::vec3( 1, 0,-1/root_two<float>());
  point[1] = glm::vec3(-1, 0,-1/root_two<float>());
  point[2] = glm::vec3( 0, 1, 1/root_two<float>());
  point[3] = glm::vec3( 0,-1, 1/root_two<float>());

  normal[0] = trianglenormal(point[0],point[1],point[2]);
  normal[1] = trianglenormal(point[1],point[3],point[2]);
  normal[2] = trianglenormal(point[2],point[3],point[0]);
  normal[3] = trianglenormal(point[3],point[1],point[0]);
  
  auto AppendSide =
    [&](int p0,int p1,int p2,int n) {
      Tetra.Append(point[p0],normal[0]);
      Tetra.Append(point[p1],normal[0]);
      Tetra.Append(point[p2],normal[0]);
    };

  AppendSide(0,1,2,0);
  AppendSide(1,2,3,1);
  AppendSide(2,3,0,2);
  AppendSide(3,0,1,3);
  
  return Tetra;
}

// still some problems with normals when mesh is called with arguments
// less than (x,40,40)
RawMesh makeSphere(float r=1,int num_steps_theta=3,int num_steps_phi=3){
  using glm::sin;
  using glm::cos;
  using glm::vec3;
  using glm::pi;

  if (num_steps_theta < 3) num_steps_theta = 3;
  if (num_steps_phi < 3)   num_steps_phi   = 3;
  
  float dtheta = pi<float>() / (num_steps_theta - 1);
  float dphi   = pi<float>() * 2 / num_steps_phi;
  
  vec3 normal = vec3(0,0,1); //top at index 0

  RawMesh sphere;
  sphere.Append(r * normal,normal);
  
  for (float theta = dtheta;theta < pi<float>();theta += dtheta) {
    for (float phi = 0;phi < 2*pi<float>();phi += dphi) {
      normal = vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
      sphere.Append(r*normal,normal);
    }
  }
  normal = vec3(0,0,-1); // bottom at last index
  sphere.Append(r * normal,normal);

  int num_points = sphere.Size();
  RawMesh::vector_Index indexlist;

  auto PushTriple =
    [&indexlist](int a,int b,int c) {
      indexlist.push_back(a);
      indexlist.push_back(b);
      indexlist.push_back(c);
    };

  // for the uppermost loop
  for (auto i = 1;i < num_steps_phi;++i) PushTriple(0,i,i + 1); 
  PushTriple(0,num_steps_phi,1);

  if (num_steps_theta > 3) {
    for (auto i = num_steps_phi + 1;i < num_points - 1;++i) {
      if (i%num_steps_phi == 0) {
	PushTriple(i,i - num_steps_phi,i + 1 - 2*num_steps_phi);
	PushTriple(i,i + 1 - num_steps_phi,i + 1 - 2*num_steps_phi);
      } else {
	PushTriple(i,i - num_steps_phi,i + 1 - num_steps_phi);
	PushTriple(i,i + 1 - num_steps_phi,i + 1);
      }
    }
  }
  // for the lowest loop
  for (int i = num_points - num_steps_phi - 1;i < num_points - 2;++i)
    PushTriple(num_points - 1,i,i + 1);
  PushTriple(num_points - 1,num_points - 2,num_points - num_steps_phi - 1);

  sphere.IndexAppend(indexlist.begin(),indexlist.end());
  return sphere;
}

RawMesh makeCylinder(float r=1,float z=1,int num_steps_phi=3) {
  using glm::vec3;
  using glm::pi;

  float dphi = pi<float>() * 2 / num_steps_phi;

  RawMesh cylinder;
  cylinder.Append(vec3(0,0,z),vec3(0,0, 1)); // top
	
  glm::vec2 xy;
  for (float phi = 0;phi < 2*pi<float>();phi += dphi) {
    xy = glm::vec2(glm::cos(phi),glm::sin(phi));
    cylinder.Append(vec3(r * xy, z),vec3(0,0, 1));
    cylinder.Append(vec3(r * xy, z),vec3( xy, 0));
    cylinder.Append(vec3(r * xy,-z),vec3( xy, 0));
    cylinder.Append(vec3(r * xy,-z),vec3(0,0,-1));
  }
  cylinder.Append(vec3(0,0,-z),vec3(0,0,-1));	// bottom

  int num_points = cylinder.Size();

  RawMesh::vector_Index indexlist;
  auto top    = 0;
  auto bottom = num_points - 1;

  auto PushTriple =
    [&indexlist](int a,int b,int c) {
      indexlist.push_back(a);
      indexlist.push_back(b);
      indexlist.push_back(c);
    };

  for (auto i = 1;i < num_points - 5;i += 4) {
    PushTriple(top,   i,    i + 4);
    PushTriple(i + 1, i + 2,i + 5);
    PushTriple(i + 6, i + 5,i + 2);
    PushTriple(bottom,i + 7,i + 3);
  }
  // last loop back to first indices
  PushTriple(top,           num_points - 5,1);
  PushTriple(num_points - 4,num_points - 3,2);
  PushTriple(3,             2,             num_points - 3);
  PushTriple(bottom,        4,             num_points - 2);

  cylinder.IndexAppend(indexlist.begin(),indexlist.end());
  
  return cylinder;
}

GRAPHICS_END

#endif//MFL_GRAPHICS_MAKE_RAW_MESH_INCLUDED
