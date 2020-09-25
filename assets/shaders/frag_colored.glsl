#version 330 core
in vec3 fragPos;
in vec3 color;

out vec4 fragColor;

void main()
{
//   vec3 dcolor = vec3(1.0, 0.0, 0.0);
//   fragColor = vec4(dcolor, 1.0);
  fragColor = vec4(color, 1.0);
}
