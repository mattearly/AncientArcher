#version 330 core
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;

out vec3 fragPos;
out vec4 fragColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
  fragPos = vec3(model * vec4(inPos, 1.0));
  fragColor = inColor;

  gl_Position = projection * view * model * vec4(inPos, 1.0);
}
