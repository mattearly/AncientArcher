#version 330 core

in vec2 pass_TexUV;

out vec4 out_Color;

struct Material
{
	sampler2D Albedo;                        // or Diffuse for a more 'shadowy' version
	
	// sampler2D TextureGloss;               // or Roughness
	// sampler2D TextureNormal;
	// sampler2D TextureAmbientOcclusion;    // more shadows
	// sampler2D TextureSpecular;            // aka Refelction for non-PBR spec workflows. PBR uses the Gloss/Roughness for this as everything is has shine
	// sampler2D TextureDisplacement;        // for rocks and ground and stuff
	// vec3 Emissive;
	// float Shininess;
};

uniform Material material;

void main()
{
	out_Color = texture(material.Albedo, pass_TexUV);	
}
