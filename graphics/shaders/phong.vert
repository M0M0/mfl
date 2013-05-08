#version 130
uniform mat4 mMVP;
uniform mat4 mMV;
uniform vec3 vLightPos;

#ifdef LAYOUT_IDENTIFIERS
layout (location=0)
#endif
in vec3 vPosition;
#ifdef LAYOUT_IDENTIFIERS
layout (location=1)
#endif
in vec3 vNormal;
#ifdef LAYOUT_IDENTIFIERS
layout (location=2)
#endif
in vec4 vColor;

smooth out vec4 vVaryingColor;
smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;

void main(){
  // normal will just be rotated and not translated
  // therefore taking the rotational part of the MV matrix
  // will suffice
  mat3 mNorm = mat3(mMV[0].xyz,mMV[1].xyz,mMV[2].xyz);
  vVaryingNormal = mNorm * vNormal;

  vec4 tmp = mMV*vec4(vPosition,1);
  tmp.xyz = tmp.xyz/tmp.w;
  vVaryingLightDir = normalize(vLightPos-tmp.xyz);
  vVaryingColor = vColor;
  gl_Position = mMVP*vec4(vPosition,1);
}