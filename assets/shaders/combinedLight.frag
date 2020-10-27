#version 330 core

in vec3 pass_Pos;
in vec2 pass_TexUV;
in vec3 pass_Norm;

out vec3 out_Color;

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

struct DirectionalLight
{
	vec3 Direction;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

struct PointLight
{
	vec3 Position;
	float Constant,Linear,Quadratic;
	vec3 Ambient,Diffuse,Specular;
};

struct SpotLight
{
	vec3 Position,Direction;
	float CutOff,OuterCutOff;
	float Constant,Linear,Quadratic;
	vec3 Ambient,Diffuse,Specular;
};

const int MAXPOINTLIGHTS=50;
const int MAXSPOTLIGHTS=25;

uniform vec3 viewPos;
uniform Material material;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight[MAXPOINTLIGHTS];
uniform SpotLight spotLight[MAXSPOTLIGHTS];

uniform int NUM_POINT_LIGHTS;
uniform int NUM_SPOT_LIGHTS;

vec3 CalcDirectionalLight(vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 viewDir);

int i = 0;

void main()
{
	vec3 result;

	vec3 normal = normalize(pass_Norm);
	vec3 viewDir = normalize(viewPos - pass_Pos);

	result += CalcDirectionalLight(normal, viewDir);
	
	for (i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLight[i], normal, viewDir);
	}

	for (i = 0; i < NUM_SPOT_LIGHTS; i++)
	{
		result += CalcSpotLight(spotLight[i], normal, viewDir);
	}

	out_Color = result;
}

vec3 CalcDirectionalLight(vec3 normal,vec3 viewDir)
{
	vec3 lightDir=normalize(-directionalLight.Direction);
	float diff=max(dot(normal,lightDir),0.);
	vec3 reflectDir=reflect(-lightDir,normal);
	// float spec=pow(max(dot(viewDir,reflectDir),0.),material.Shininess);
	
	vec3 ambient=directionalLight.Ambient*texture(material.Albedo,pass_TexUV).rgb;
	vec3 diffuse=directionalLight.Diffuse*diff*texture(material.Albedo,pass_TexUV).rgb;
	
	// vec3 specular = directionalLight.Specular * spec * texture(material.Specular, pass_TexUV).rgb;
	
	return(ambient+diffuse/*+ specular*/);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Position - pass_Pos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	
	float dist = length(light.Position - pass_Pos);
	float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));
	
	vec3 ambient = light.Ambient * texture(material.Albedo, pass_TexUV).rgb;
	vec3 diffuse = light.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;
	//  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, pass_TexUV);
	
	ambient *= attenuation;
	diffuse *= attenuation;
	//  specular *= attenuation;
	
	return (ambient + diffuse /*+ specular*/);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Position - pass_Pos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
	
	float dist = length(light.Position - pass_Pos);
	float attenuation  = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));
	
	float theta = dot(lightDir, normalize(-light.Direction));
	float epsilon = light.CutOff - light.OuterCutOff;
	float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
	
	vec3 ambient = light.Ambient * texture(material.Albedo, pass_TexUV).rgb;
	vec3 diffuse = light.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;
	
	//  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, pass_TexUV);
	
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	//  specular *= attenuation * intensity;
	
	return (ambient + diffuse /*+ specular*/);
}
