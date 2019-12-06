#version 330 core
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 color;

void main()
{
  fragColor = vec4(color, 1.0);
}
