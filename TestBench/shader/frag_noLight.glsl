#version 330 core
in vec3 fragPos;
in vec2 texUV;
in vec3 norm; //unused

out vec4 fragColor;

struct Material
{
  sampler2D diffuse;
};

uniform Material material;

void main()
{
  vec3 result = texture(material.diffuse, texUV).rgb;
  fragColor = vec4(result, 1.0);
}
