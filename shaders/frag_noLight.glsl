#version 330 core
in vec3 fragPos;
in vec2 texUV;
in vec3 norm; //unused
in vec3 color;

out vec4 fragColor;

struct Material
{
  sampler2D diffuse;
};

uniform Material material;

void main()
{
 // const vec4 ambient(.2, .2, .2, 1.0); 
  vec4 result =  texture(material.diffuse, texUV);
//  result *= ambient;
  
  fragColor = result;
}
