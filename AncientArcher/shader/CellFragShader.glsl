#version 330 core
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 color;

void main()
{
//  vec3 tmpcolor = vec3(.5, .1, .1);
  fragColor = vec4(color, 1.0);
}
