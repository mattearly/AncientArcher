#version 430 core

in vec2 pass_TexUV;

out vec4 out_Color;

struct Material
{
  sampler2D Albedo;
};

uniform Material material;

void main()
{
  out_Color = texture(material.Albedo, pass_TexUV);
}
