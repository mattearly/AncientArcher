#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirectionalLight
{
vec3 direction;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dirLight;

vec3 CalcDirectionalLight(DirectionalLight inLight, vec3 inNormal, vec3 inViewDir);

void main()
{
  vec3 normal = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 result = CalcDirectionalLight(dirLight, normal, viewDir);

  FragColor = vec4(result, 1.0);
 
}

vec3 CalcDirectionalLight(DirectionalLight inLight, vec3 inNormal, vec3 inViewDir) {

  vec3 lightDir = normalize(-inLight.direction);
  float diff = max(dot(inNormal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, inNormal);

//  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  float spec = pow(max(dot(inViewDir, reflectDir), 0.0), 0.f);

  // combine results
  vec3 ambient = inLight.ambient * vec3(texture(material.diffuse, TexCoords));
  vec3 diffuse = inLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));;
  vec3 specular = inLight.specular * spec * vec3(texture(material.specular, TexCoords));

  return (ambient + diffuse + specular);
}
