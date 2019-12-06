#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexUV;

out vec3 fragPos;
out vec2 texUV;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
  fragPos = vec3(model * vec4(inPos, 1.0));
  texUV = inTexUV;

  gl_Position = projection * view * model * vec4(inPos, 1.0);
}