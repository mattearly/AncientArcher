#version 430 core
// passing in from combinedLight.vert
layout(location = 0) out vec4 pass_Pos;
layout(location = 1) out vec4 pass_Norm;
layout(location = 2) out vec4 pass_Color;
layout(location = 3) out vec2 pass_TexUV;

// final out pixel color
out vec4 out_Color;

// ------------ STRUCT DECLARACTIONS
struct Material
{
  sampler2D TextureUnit;
  // int TextureCount;
  // vec4 Diffuse;
  // vec4 Ambient;
  vec4 Specular;
  // vec4 Emissive;
  float Shininess;
};

struct DirectionalLight
{
  vec4 Direction;
  vec4 Ambient;
  vec4 Diffuse;
  vec4 Specular; 
};

struct PointLight
{
  vec4 Position;
  float Constant, Linear, Quadratic;
  vec4 Ambient, Diffuse, Specular;
};

struct SpotLight
{
  vec4 Position, Direction;
  float CutOff, OuterCutOff;
  float Constant, Linear, Quadratic;
  vec4 Ambient, Diffuse, Specular;
};

//  constants
const int MAXPOINTLIGHTS = 50;
const int MAXSPOTLIGHTS = 25;

// ----------- UNIFORM DECLARATIONS
uniform vec4           viewPos;
uniform Material       material;

uniform DirectionalLight directionalLight;
uniform PointLight       pointLight[MAXPOINTLIGHTS];
uniform SpotLight        spotLight[MAXSPOTLIGHTS];

uniform int NUM_POINT_LIGHTS;
uniform int NUM_SPOT_LIGHTS;


// ---------- FUNCTION DELCARATIONS
vec4 CalcDirectionalLight(vec4 normal, vec4 viewDir);
vec4 CalcPointLight(PointLight light, vec4 normal, vec4 viewDir);
vec4 CalcSpotLight(SpotLight light, vec4 normal, vec4 viewDir);


// --------- MAIN
void main()
{
  vec4 normal = normalize(pass_Norm);
  vec4 viewDir = normalize(viewPos - pass_Pos);

  vec4 result;
  
 // calc directional light on fragment
  result += CalcDirectionalLight(normal, viewDir);

  // calc point lights on fragments
  for (int i = 0; i < NUM_POINT_LIGHTS; i++)
    result += CalcPointLight(pointLight[i], normal, viewDir);

  // calc spot lights on the fragments
  for (int i = 0; i < NUM_SPOT_LIGHTS
; i++) 
    result += CalcSpotLight(spotLight[i], normal, viewDir);

  out_Color = result;
}


// -------- FUNCTION DEFINITIONS
vec4 CalcDirectionalLight(vec4 normal, vec4 viewDir)
{
  vec4 lightDir = normalize(-directionalLight.Direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec4 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

  vec4 ambient = directionalLight.Ambient * texture(material.TextureUnit, pass_TexUV).rgba;
  vec4 diffuse = directionalLight.Diffuse * diff * texture(material.TextureUnit, pass_TexUV).rgba;

  // vec4 specular = directionalLight.Specular * spec * texture(material.Specular, pass_TexUV).rgba;

  return (ambient + diffuse /*+ specular*/);
}

vec4 CalcPointLight(PointLight light, vec4 normal, vec4 viewDir)
{
  vec4 lightDir = normalize(light.Position - pass_Pos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec4 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

  float dist = length(light.Position - pass_Pos);
  float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));

  vec4 ambient = light.Ambient * texture(material.TextureUnit, pass_TexUV).rgba;
  vec4 diffuse = light.Diffuse * diff * texture(material.TextureUnit, pass_TexUV).rgba;
//  vec4 specular = light.Specular * spec * vec4(texture(material.Specular, pass_TexUV);

  ambient *= attenuation;
  diffuse *= attenuation;
//  specular *= attenuation;

  return (ambient + diffuse /*+ specular*/);
}

vec4 CalcSpotLight(SpotLight light, vec4 normal, vec4 viewDir)
{
  vec4 lightDir = normalize(light.Position - pass_Pos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec4 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);

  float dist = length(light.Position - pass_Pos); 
  float attenuation  = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));  

  float theta = dot(lightDir, normalize(-light.Direction));
  float epsilon = light.CutOff - light.OuterCutOff;
  float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

  vec4 ambient = light.Ambient * texture(material.TextureUnit, pass_TexUV).rgba;
  vec4 diffuse = light.Diffuse * diff * texture(material.TextureUnit, pass_TexUV).rgba;

//  vec4 specular = light.Specular * spec * vec4(texture(material.Specular, pass_TexUV);

  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
//  specular *= attenuation * intensity;

  return (ambient + diffuse /*+ specular*/);
}
