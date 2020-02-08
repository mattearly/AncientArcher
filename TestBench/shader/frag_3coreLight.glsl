#version 330 core
in vec3 fragPos;
in vec2 texUV;
in vec3 norm;

out vec4 fragColor;

struct Material
{
  sampler2D diffuse;
};

struct DirectionalLight
{
  vec3 Direction, Ambient, Diffuse, Specular;
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

uniform vec3 viewPos;

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
//  vec4 color = texture(material.diffuse, texUV);

  vec3 normal = normalize(norm);
  vec3 viewDir = normalize(viewPos - fragPos);

  vec3 result;

  result += CalcDirectionalLight(dirLight, normal, viewDir);

  result += CalcPointLight(pointLight, normal, viewDir);

  result += CalcSpotLight(spotLight, normal, viewDir);

  fragColor = vec4(result, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.Direction);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Gloss);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.0);

  vec3 ambient = light.Ambient * texture(material.diffuse, texUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.diffuse, texUV).rgb;
//  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, texUV);

  return (ambient + diffuse /*+ specular*/);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(light.Position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Gloss);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.0);

  float dist = length(light.Position - fragPos);
  float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));

  vec3 ambient = light.Ambient * texture(material.diffuse, texUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.diffuse, texUV).rgb;
//  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, texUV);

  ambient *= attenuation;
  diffuse *= attenuation;
//  specular *= attenuation;

  return (ambient + diffuse /*+ specular*/);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightDir = normalize(light.Position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Gloss);
//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 0.0);

  float dist = length(light.Position - fragPos); 
  float attenuation  = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));  

  float theta = dot(lightDir, normalize(-light.Direction));
  float epsilon = light.CutOff - light.OuterCutOff;
  float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

  vec3 ambient = light.Ambient * texture(material.diffuse, texUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.diffuse, texUV).rgb;
//  vec3 specular = light.Specular * spec * vec3(texture(material.Specular, texUV);

  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
//  specular *= attenuation * intensity;

  return (ambient + diffuse /*+ specular*/);
}
