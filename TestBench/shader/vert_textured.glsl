#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexUV;
layout(location = 2) in vec3 inNorm;

out vec3 fragPos;
out vec2 texUV;
out vec3 norm;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
  fragPos = vec3(model * vec4(inPos, 1.0));
  texUV = inTexUV;
  norm = inNorm;

  gl_Position = projection * view * model * vec4(inPos, 1.0);
}
