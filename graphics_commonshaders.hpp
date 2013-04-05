#ifndef MFL_GRAPHICS_COMMONSHADERS_INCLUDED
#define MFL_GRAPHICS_COMMONSHADERS_INCLUDED

#include "MFL_Graphics_Common.hpp"
#define STRINGIFY(x) #x

GRAPHICS_BEGIN

struct ShaderPair{
	const char* vertex;
	const char* fragment;
};

// ======================================================
// Gourand Shader:
// Inputs  : Vertex, Normal, Color
// Uniforms: MVPMatrix,MVMatrix,NormalMatrix, LightVector
// ======================================================
ShaderPair GourandShader = {

// =====================================================
// Vertex Shader:
// =====================================================
"#version 130\n"
STRINGIFY(
uniform mat4 mMVP;
uniform mat4 mMV;

uniform vec3 vLightPos;

layout (location=0) in vec3 vVertex;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec4 vColor;

smooth out vec4 vVaryingColor;

void main(){
	// normal will just be rotated and not translated
	// therefore taking the rotational part of the MV matrix
	// will suffice
	mat3 mNorm = mat3( mMV[0].xyz, mMV[1].xyz, mMV[2].xyz );
	vec3 vEyeNormal = mNorm*vNormal;

	vec4 tmp = mMV*vec4(vVertex,1);
	tmp.xyz = tmp.xyz/tmp.w;

	vec3 vLightDir = normalize(vLightPos-tmp.xyz);

	float diff = dot(vEyeNormal,vLightDir);

	vVaryingColor = diff*vColor;

	vVaryingColor += vColor*0.5f;
	
	if(diff != 0){

		vec3 vReflection = normalize(reflect(-vLightDir,vEyeNormal));
		float spec = max(0.0,dot(vEyeNormal,vReflection));

		float fSpec = pow(spec,128.0f);
		vVaryingColor.rgb += vec3(fSpec);
	}

	gl_Position = mMVP*vec4(vVertex,1);
}),

// ======================================================
// Fragment Shader:
// ======================================================
"#version 130\n"
STRINGIFY(
smooth in vec4 vVaryingColor;
layout (location=0) out vec4 color_out;

void main(){
    color_out = vVaryingColor;
})
};

// ======================================================
// Phong  Shader:
// Inputs  : Vertex, Normal, Color
// Uniforms: MVPMatrix,MVMatrix,NormalMatrix, LightVector
// ======================================================
ShaderPair PhongShader = { 

// ======================================================
// Vertex Shader:
// ======================================================
"#version 130\n"
STRINGIFY(
uniform mat4 mMVP;
uniform mat4 mMV;
//uniform mat3 mNorm;

uniform vec3 vLightPos;

layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec4 vColor;

smooth out vec4 vVaryingColor;
smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;

void main(){
	// normal will just be rotated and not translated
	// therefore taking the rotational part of the MV matrix
	// will suffice
	mat3 mNorm = mat3( mMV[0].xyz, mMV[1].xyz, mMV[2].xyz );
	vVaryingNormal = mNorm * vNormal;

	vec4 tmp = mMV*vec4(vPosition,1);
	tmp.xyz = tmp.xyz/tmp.w;	
	vVaryingLightDir = normalize(vLightPos-tmp.xyz);
	vVaryingColor = vColor;
	gl_Position = mMVP*vec4(vPosition,1);
}),

// ======================================================
// Fragment Shader:
// ====================================================== 
"#version 130\n"
STRINGIFY(
smooth in vec4 vVaryingColor;
smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;
layout (location=0) out vec4 color_out;

void main(){
	// for readability and optimisation!
	vec3 vNormal = normalize(vVaryingNormal);
	vec3 vLightDir = normalize(vVaryingLightDir);

	float diff = max(0.0,dot(vNormal,vLightDir));

	color_out = diff * vVaryingColor;

	color_out += vVaryingColor*0.1f;
	
	if(diff != 0){
		vec3 vReflection = 
			normalize( reflect(-vLightDir,vNormal) );
		float spec = max(0.0,dot(vNormal,vReflection));

		float fSpec = pow(spec,128.0f);
		color_out.rgb += vec3(fSpec);
	}
})
};
// ======================================================
// Flat Shader:
//	 Will make every model white
//
// Inputs  : Vertex
// Uniforms: MVPMatrix
// ======================================================
ShaderPair FlatShader = {
"#version 130\n"
STRINGIFY(
uniform mat4 mMVP;

layout (location=0) in vec3 vPosition;

void main(){
	gl_Position = mMVP*vec4(vPosition,1);
}
),
"#version 130\n"
STRINGIFY(
layout (location=0) out vec4 color_out;
void main(){
	color_out = vec4(1);
})	
};



GRAPHICS_END

#endif//MFL_GRAPHICS_COMMONSHADERS_INCLUDED