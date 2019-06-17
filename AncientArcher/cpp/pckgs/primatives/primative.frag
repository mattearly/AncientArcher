#version 330 core

out vec4 FragColor;

struct Material {
  //sampler2D diffuse;  // diffuse will be whatever texture we are using from texture0
  //sampler2D specular;  // specular comes from diffuseTextureBank
  float shininess;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

//struct SpotLight {
//  vec3 position;
//  vec3 direction;
//  float cutOff;
//  float outerCutOff;
//
//  float constant;
//  float linear;
//  float quadratic;
//
//  vec3 ambient;
//  vec3 diffuse;
//  vec3 specular;
//};


in vec3 FragPos;
in vec2 texCoords;
in vec3 Normal;

//uniform int specnum;
//uniform sampler2D specular0;  // no specular value | GLTexture16
// texture samplers switcher for texture bank
//uniform int activeTex;
// texture samplers from texture bank
uniform sampler2D texture0;  // GLTexture0
//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform sampler2D texture3;
//uniform sampler2D texture4;
//uniform sampler2D texture5;
//uniform sampler2D texture6;
//uniform sampler2D texture7;
//uniform sampler2D texture8;
//uniform sampler2D texture9;
//uniform sampler2D texture10;
//uniform sampler2D texture11;
//uniform sampler2D texture12;
//uniform sampler2D texture13;
//uniform sampler2D texture14;
//uniform sampler2D texture15;  // GLTexture15
//uniform sampler2D texture16;  
//uniform sampler2D texture17;
//uniform sampler2D texture18;
//uniform sampler2D texture19;
//uniform sampler2D texture20;
//uniform sampler2D texture21;
//uniform sampler2D texture22;
//uniform sampler2D texture23;
//uniform sampler2D texture24;
//uniform sampler2D texture25;
//uniform sampler2D texture26;
//uniform sampler2D texture27;
//uniform sampler2D texture28;
//uniform sampler2D texture29;
//uniform sampler2D texture30;
//uniform sampler2D texture31;

//uniform vec3 lightviewDir;
//uniform vec3 lightPos;

uniform vec3 viewPos;

uniform DirLight dirLight;

uniform int numPointLights;
uniform PointLight pointLight[4];
//uniform SpotLight spotLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 color);
vec3 color;

void main() {

color = texture(texture0, texCoords).rgb;
   //texture bank switch
//  switch (activeTex) {
//  case 0: color = texture(texture0, texCoords).rgb; break;
//  case 1: color = texture(texture1, texCoords).rgb; break;
//  case 2: color = texture(texture2, texCoords).rgb; break;
//  case 3: color = texture(texture3, texCoords).rgb; break;
//  case 4: color = texture(texture4, texCoords).rgb; break;
//  case 5: color = texture(texture5, texCoords).rgb; break;
//  case 6: color = texture(texture6, texCoords).rgb; break; 
//  case 7: color = texture(texture7, texCoords).rgb; break; 
//  case 8: color = texture(texture8, texCoords).rgb; break; 
//  case 9: color = texture(texture9, texCoords).rgb; break; 
//  case 10: color = texture(texture10, texCoords).rgb; break; 
//  case 11: color = texture(texture11, texCoords).rgb; break; 
//  case 12: color = texture(texture12, texCoords).rgb; break; 
//  case 13: color = texture(texture13, texCoords).rgb; break; 
//  case 14: color = texture(texture14, texCoords).rgb; break; 
//  case 15: color = texture(texture15, texCoords).rgb; break; 
//  case 16: color = texture(texture16, texCoords).rgb; break; 
//  case 17: color = texture(texture17, texCoords).rgb; break; 
//  case 18: color = texture(texture18, texCoords).rgb; break; 
//  case 19: color = texture(texture19, texCoords).rgb; break; 
//  case 20: color = texture(texture20, texCoords).rgb; break; 
//  case 21: color = texture(texture21, texCoords).rgb; break; 
//  case 22: color = texture(texture22, texCoords).rgb; break; 
//  case 23: color = texture(texture23, texCoords).rgb; break; 
//  case 24: color = texture(texture24, texCoords).rgb; break; 
//  case 25: color = texture(texture25, texCoords).rgb; break; 
//  case 26: color = texture(texture26, texCoords).rgb; break; 
//  case 27: color = texture(texture27, texCoords).rgb; break; 
//  case 28: color = texture(texture28, texCoords).rgb; break; 
//  case 29: color = texture(texture29, texCoords).rgb; break; 
//  case 30: color = texture(texture30, texCoords).rgb; break; 
//  case 31: color = texture(texture31, texCoords).rgb; break; 
//  }

  //vec3 ambient = 0.17 * lightviewDir * color;  // the darkest we ever want to get is 0.17
  //FragColor = vec4(ambient, 1.0);

  // properties
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  // phase 1: directional lighting
  vec3 result = CalcDirLight(dirLight, norm, viewDir);

  // phase 2: point lights
  for (int i = 0; i < numPointLights; i++)
    result += CalcPointLight(pointLight[i], norm, FragPos, viewDir);

  // phase 3: spot light
  //vec3 result = CalcSpotLight(spotLight, norm, FragPos, color);

  FragColor = vec4(result, 1.0);

}

// calculates the viewDir when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

  vec3 lightDir = normalize(-light.direction);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // combine results
  vec3 ambient0 = light.ambient * color;
  vec3 diffuse0 = light.diffuse * diff * color;

  //vec3 specular;
  //switch (specnum) {
  //case 0:
  //  specular = light.specular * spec * vec3(texture(specular0, texCoords));
  //  break;
  //}

  return (ambient0 + diffuse0 /* + specular*/);
}

// calculates the viewDir when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

  vec3 lightDir = normalize(light.position - fragPos);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);

  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  // attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
  
  // combine results
  vec3 ambient0 = light.ambient * color;
  vec3 diffuse0 = light.diffuse * diff * color;

  //vec3 specular;
  //switch (specnum) {
  //case 0:
  //  specular = light.specular * spec * vec3(texture(specular0, texCoords));
  //  break;
  //} 
  
  ambient0 *= attenuation;
  diffuse0 *= attenuation;
  //specular *= attenuation;

  return (ambient0 + diffuse0 /*+ specular*/);
}

// calculates the viewDir when using a spot light.
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 color) {
//
//  vec3 lightDir = normalize(light.position - fragPos);
//
//  // diffuse shading
//  float diff = max(dot(normal, lightDir), 0.0);
//
//  // specular shading
//  vec3 reflectDir = reflect(-lightDir, normal);
//  float spec = pow(max(dot(color, reflectDir), 0.0), material.shininess);
//
//  // attenuation
//  float distance = length(light.position - fragPos);
//  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//  
//  // spotlight intensity
//  float theta = dot(lightDir, normalize(-light.direction));
//  float epsilon = light.cutOff - light.outerCutOff;
//  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//
//  // combine results
//  vec3 ambient0 = light.ambient * color;
//  vec3 diffuse0 = light.diffuse * diff * color;
//
//  vec3 specular;
//  switch (specnum) {
//  case 0:
//    specular = light.specular * spec * vec3(texture(specular0, texCoords));
//    break;
//  } 
//  
//  ambient0 *= attenuation * intensity;
//  diffuse0 *= attenuation * intensity;
//  specular *= attenuation * intensity;
//
//  return (ambient0 + diffuse0 + specular);
//}