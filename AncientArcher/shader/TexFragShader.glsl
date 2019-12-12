#version 330 core
in vec3 fragPos;
in vec2 texUV;
in vec3 norm;

out vec4 fragColor;

struct Material
{
  sampler2D diffuse1;
//  vec4 Gloss; // xyz = norm, w = gloss
  float Gloss;
  sampler2D Specular;
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

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 color);

void main()
{
  vec4 color = texture(material.diffuse1, texUV);

  vec3 normal = normalize(norm);
  vec3 viewDir = normalize(viewPos - fragPos);

  vec3 result = CalcDirectionalLight(dirLight, normal, viewDir, vec3(color.rgb));
  
  fragColor = vec4(result, 1.f);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 color)
{
  vec3 lightDirection = normalize(-light.Direction);
  float diff = max(dot(normal, lightDirection), 0.f);
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDir, reflectDirection), 0.f), 0.f);

  vec3 ambient = light.Ambient * color;
  vec3 diffuse = light.Diffuse * diff * color;
  vec3 specular = light.Specular * spec * color;

  return (ambient * diffuse * specular);
}