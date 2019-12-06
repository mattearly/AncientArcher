#version 330 core
in vec3 fragPos;
in vec2 texUV;

out vec4 fragColor;

struct Material
{
  sampler2D diffuse1;
  sampler2D diffuse2;
};

uniform Material material;

void main()
{
  vec4 color = texture(material.diffuse1, texUV);

  fragColor = color;
}