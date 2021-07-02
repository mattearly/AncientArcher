#pragma once
#include <string>
namespace AA {
const std::string frag_diff = R"(
#version 430 core

in vec2 pass_TexUV;

out vec4 out_Color;

struct Material {
  sampler2D Albedo;
};

uniform Material material;

void main() {
  out_Color = texture(material.Albedo, pass_TexUV);
}
)";

const std::string frag_lit = R"(
#version 430 core

in vec3 pass_Pos;
in vec3 pass_Norm;
in vec2 pass_TexUV;
in vec3 pass_Tangent;
in vec3 pass_BiTangent;

out vec4 out_Color;

struct Material {
  sampler2D Albedo;
  sampler2D Specular;
//  sampler2D Normal;
  float Shininess;
};

struct DirectionalLight {
  vec3 Direction;
  vec3 Ambient;
  vec3 Diffuse;
  vec3 Specular;
};

struct PointLight {
  vec3 Position;
  float Constant, Linear, Quadratic;
  vec3 Ambient, Diffuse, Specular;
};

struct SpotLight {
  vec3 Position, Direction;
  float CutOff, OuterCutOff;
  float Constant, Linear, Quadratic;
  vec3 Ambient, Diffuse, Specular;
};

const int MAXPOINTLIGHTS = 50;
const int MAXSPOTLIGHTS = 25;

uniform vec3 viewPos;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform int isDirectionalLightOn;
uniform PointLight pointLight[MAXPOINTLIGHTS];
uniform SpotLight spotLight[MAXSPOTLIGHTS];
uniform int NUM_POINT_LIGHTS;
uniform int NUM_SPOT_LIGHTS;

vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-directionalLight.Direction);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.), material.Shininess);
  // combine results
  vec3 ambient = directionalLight.Ambient * texture(material.Albedo, pass_TexUV).rgb;
  vec3 diffuse = directionalLight.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;
  vec3 specular = directionalLight.Specular * spec * texture(material.Specular, pass_TexUV).rgb;
  return(ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir){
  vec3 lightDir = normalize(light.Position - pass_Pos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
  // attenuation
  float dist = length(light.Position - pass_Pos);
  float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));
  // combine results
  vec3 ambient = light.Ambient * texture(material.Albedo, pass_TexUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;
  vec3 specular = light.Specular * spec * texture(material.Specular, pass_TexUV).rgb;
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir){
  vec3 lightDir = normalize(light.Position - pass_Pos);
  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shaing
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
  // attenuation
  float dist = length(light.Position - pass_Pos);
  float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * (dist * dist));
  // cone of light
  float theta = dot(lightDir, normalize(-light.Direction));
  float epsilon = light.CutOff - light.OuterCutOff;
  float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
  // combine results
  vec3 ambient = light.Ambient * texture(material.Albedo, pass_TexUV).rgb;
  vec3 diffuse = light.Diffuse * diff * texture(material.Albedo, pass_TexUV).rgb;
  vec3 specular = light.Specular * spec * texture(material.Specular, pass_TexUV).rgb;
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular);
}

void main()
{
  vec3 result;
  vec3 normal = normalize(pass_Norm);
  vec3 viewDir = normalize(viewPos - pass_Pos);

  if (isDirectionalLightOn > 0)
    result += CalcDirectionalLight(normal, viewDir);

  int i = 0;
  for (i; i < NUM_POINT_LIGHTS; i++)
    result += CalcPointLight(pointLight[i], normal, viewDir);

  for (i = 0; i < NUM_SPOT_LIGHTS; i++)
    result += CalcSpotLight(spotLight[i], normal, viewDir);

  out_Color = vec4(result, 1.0);
}
)";

const std::string frag_skycube = R"(
#version 430 core
in vec3 TexCoords;
out vec4 FragColor;
uniform samplerCube skybox;
void main() {
  FragColor = texture(skybox, TexCoords);
}
)";

const std::string vert_3D = R"(
#version 430 core

layout(location=0)in vec3 inPos;
layout(location=1)in vec3 inNorm;
layout(location=2)in vec2 inTexUV;
layout(location=3)in vec3 inTangent;
layout(location=4)in vec3 inBiTangent;
//layout(location=5)in ivec4 inBoneIds;
//layout(location=6)in vec4 inWeights;

out vec3 pass_Pos;
out vec3 pass_Norm;
out vec2 pass_TexUV;
out vec3 pass_Tangent;
out vec3 pass_BiTangent;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

//const int MAX_BONES = 100;
//uniform mat4 finalBoneMatrices[MAX_BONES];

//const int MAX_BONE_INFLUENCE = 4;

//uniform bool isAnimated;
//uniform bool isLit;

void main()
{
  pass_Pos = (u_model_matrix * vec4(inPos, 1.0)).xyz;
  pass_Norm = mat3(transpose(inverse(u_model_matrix))) * inNorm;
//  pass_Norm = mat3((u_model_matrix)) * inNorm;
  pass_TexUV = inTexUV;
  pass_Tangent = inTangent;
  pass_BiTangent = inBiTangent;

  vec4 final_pos = u_projection_matrix * u_view_matrix * vec4(pass_Pos, 1);

  gl_Position = final_pos;

//  if (isAnimated) {
//    vec4 totalPosition = vec4(0.0f);
//    vec3 totalNormal = vec3(0.0f);
//
//    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
//      if(inBoneIds[i] == -1)
//        continue;
//
//      if(inBoneIds[i] >= MAX_BONES){
//        totalPosition = vec4(inPos,1.0f);
//        break;
//      }
//
//      vec4 localPosition = finalBoneMatrices[inBoneIds[i]] * vec4(inPos,1.0f);
//      totalPosition += localPosition * inWeights[i];
//
//      if (isLit) {
//        vec3 localNormal = mat3(finalBoneMatrices[inBoneIds[i]]) * inNorm;
//        totalNormal += localNormal;
//      }
//    }
//    if (isLit){
//      pass_Norm = totalNormal;
//      pass_Pos = vec3(totalPosition);
//    }
//    gl_Position =  projection * view * model * totalPosition;
//  } else {  // not animated
//    if (isLit) {
//      pass_Norm = inNorm;
//      pass_Pos = inPos;
//    }
//    gl_Position =  projection * view * model * vec4(inPos, 1);
//  }


}

)";

const std::string vert_skycube = R"(
#version 430 core
layout(location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 projection;
uniform mat4 view;
void main(){
  TexCoords = aPos;
  vec4 pos = projection * view * vec4(aPos, 1.0);
  gl_Position = pos.xyww;
}
)";

const std::string vert_2D = R"(
#version 430 core
layout(location=0)in vec2 inPos;

out vec3 pass_Pos;
out vec2 pass_TexUV;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

void main() {
  pass_Pos = (u_model_matrix * vec4(inPos, 0, 1.0)).xyz;
  pass_TexUV = vec2((inPos.x+1.0)/2.0, 1-(inPos.y+1.0)/2.0);
  vec4 final_pos = u_projection_matrix * u_view_matrix * pass_Pos;
  gl_Position = final_pos;
}
)";


}  // end namespace AA