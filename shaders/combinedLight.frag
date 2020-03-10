#version 330 core
layout(location = 0) out vec3 pass_Pos;
layout(location = 1) out vec3 pass_Norm;
layout(location = 2) out vec4 pass_Color;
layout(location = 3) out vec2 pass_TexUV;

out vec4 out_Color;

struct Material
{
  sampler2D TextureUnit;
  int TextureCount;
  vec4 Diffuse;
  vec4 Ambient;
  vec4 Specular;
  vec4 Emissive;
  float Shininess;
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
  float Constant, Linear, Quadratic;
  vec3 Ambient, Diffuse, Specular;
};

struct SpotLight
{
  vec3 Position, Direction;
  float CutOff, OuterCutOff;
  float Constant, Linear, Quadratic;
  vec3 Ambient, Diffuse, Specular;
};

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

const int MAXPOINTLIGHTS = 20;
const int MAXSPOTLIGHTS = 10;

uniform vec3           viewPos;
uniform Material       material;

uniform DirectionalLight directionalLight;
uniform PointLight       pointLight[20];
uniform SpotLight        spotLight[10];

uniform int pointLightsInUse;
uniform int spotLightsInUse;

void main()
{
  vec3 normal = normalize(pass_Norm);
  vec3 viewDir = normalize(viewPos - pass_Pos);

  vec3 result;
  
 // calc directional light on fragment
  result += CalcDirectionalLight(directionalLight, normal, viewDir);

  // calc point lights on fragments
  for (int i = 0; i < pointLightsInUse; i++)
    result += CalcPointLight(pointLight[i], normal, viewDir);

  // calc spot lights on the fragments
  for (int i = 0; i < spotLightsInUse; i++) 
    result += CalcSpotLight(spotLight[i], normal, viewDir);

  out_Color = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.Direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Gloss);

  vec3 ambient = light.Ambient * texture(material.TextureUnit, pass_TexUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.TextureUnit, pass_TexUV).rgb;

  // vec3 specular = light.Specular * spec * vec3(texture(material.Specular, pass_TexUV));

  return (ambient + diffuse /*+ specular*/);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(light.Position - pass_Pos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Gloss);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.0);

  float dist = length(light.Position - pass_Pos);
  float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));

  vec3 ambient = light.Ambient * texture(material.TextureUnit, pass_TexUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.TextureUnit, pass_TexUV).rgb;
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
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Gloss);

  float dist = length(light.Position - pass_Pos); 
  float attenuation  = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));  

  float theta = dot(lightDir, normalize(-light.Direction));
  float epsilon = light.CutOff - light.OuterCutOff;
  float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

  vec3 ambient = light.Ambient * texture(material.TextureUnit, pass_TexUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.TextureUnit, pass_TexUV).rgb;

//  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, pass_TexUV);

  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
//  specular *= attenuation * intensity;

  return (ambient + diffuse /*+ specular*/);
}
