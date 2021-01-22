#version 330 core
layout(location=0)in vec3 inPos;
layout(location=1)in vec2 inTexUV;
out vec2 pass_TexUV;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
void main(){
  pass_TexUV = inTexUV;
  gl_Position = projection * view * model * vec4(inPos,1);
}