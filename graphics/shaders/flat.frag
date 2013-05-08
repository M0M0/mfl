#version 130
#ifdef LAYOUT_IDENTIFIERS
layout (location=0)
#endif
out vec4 color_out;
void main(){
  color_out = vec4(1);
}
