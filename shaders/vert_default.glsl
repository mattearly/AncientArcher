#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexUV;
layout(location = 2) in vec3 inNorm;
layout(location = 3) in vec3 inColor;

out vec3 fragPos;
out vec2 texUV;
out vec3 norm;
out vec3 color;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
  fragPos = vec3(model * vec4(inPos, 1.0));
  texUV = inTexUV;
  norm = inNorm;
  color = inColor;

  gl_Position = projection * view * model * vec4(inPos, 1.0);
}