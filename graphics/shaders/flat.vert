#version 130
uniform mat4 mMVP;
#ifdef LAYOUT_IDENTIFIERS
//layout(location = 0)
#endif
in vec3 vPosition;
void main() {
  gl_Position = mMVP * vec4(vPosition,1);
}
