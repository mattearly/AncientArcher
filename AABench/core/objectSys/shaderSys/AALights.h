#pragma once
#include <glm/glm.hpp>

const int MAXPOINTLIGHTS = 20;
const int MAXSPOTLIGHTS = 10;
static int numPointLightsInUse = 0;
static int numSpotLightsInUse = 0;

struct DirectionalLight
{
  glm::vec3 Direction, Ambient, Diffuse, Specular;
};

struct PointLight
{
  glm::vec3 Position;
  float Constant, Linear, Quadratic;
  glm::vec3 Ambient, Diffuse, Specular;
};

struct SpotLight
{
  glm::vec3 Position, Direction;
  float CutOff, OuterCutOff;
  float Constant, Linear, Quadratic;
  glm::vec3 Ambient, Diffuse, Specular;
};

// single lights
static void setDirectionalLight(const DirectionalLight& light, const AAOGLShader& shader)
{
  shader.use();

  std::string direction, ambient, diffuse, specular;
  direction = "directionalLight.";
  ambient = diffuse = specular = direction;
  direction += "Direction";
  ambient +=   "Ambient";
  diffuse +=   "Diffuse";
  specular +=  "Specular";

  shader.setVec3(direction, light.Direction);
  shader.setVec3(ambient,   light.Ambient);
  shader.setVec3(diffuse,   light.Diffuse);
  shader.setVec3(specular,  light.Specular);
}

static void setPointLight(const PointLight& light, const AAOGLShader& shader)
{
  shader.use();

  std::string position, constant, linear, quadratic, ambient, diffuse, specular;
  position = "pointLight[].";
  constant = linear = quadratic = ambient = diffuse = specular = position;
  position +=  "Position";
  constant +=  "Constant";
  linear +=    "Linear";
  quadratic += "Quadratic";
  ambient +=   "Ambient";
  diffuse +=   "Diffuse";
  specular +=  "Specular";

  shader.setVec3(position, light.Position);
  shader.setFloat(constant, light.Constant);
  shader.setFloat(linear, light.Linear);
  shader.setFloat(quadratic, light.Quadratic);
  shader.setVec3(ambient, light.Ambient);
  shader.setVec3(diffuse, light.Diffuse);
  shader.setVec3(specular, light.Specular);
}

static void setSpotLight(const SpotLight& light, const AAOGLShader& shader)
{
  shader.use();
  std::string position, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
  position = "spotLight[].";
  ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = position;
  position +=  "Position";
  constant +=  "Constant";
  cutoff +=    "CutOff";
  ocutoff +=   "OuterCutOff";
  direction += "Direction";
  linear +=    "Linear";
  quadrat +=   "Quadratic";
  ambient +=   "Ambient";
  diffuse +=   "Diffuse";
  specular +=  "Specular";

  shader.setVec3(position, light.Position);
  shader.setFloat(cutoff, light.CutOff);
  shader.setFloat(ocutoff, light.OuterCutOff);
  shader.setVec3(direction, light.Direction);
  shader.setFloat(linear, light.Linear);
  shader.setFloat(quadrat, light.Quadratic);
  shader.setVec3(ambient, light.Ambient);
  shader.setVec3(diffuse, light.Diffuse);
  shader.setVec3(specular, light.Specular);
}


// multi lights
static void setPointLight(const PointLight& light, const int which, const AAOGLShader& shader)
{
  if (which >= MAXPOINTLIGHTS)
  {
    return;
  }
  
  if (which >= numPointLightsInUse)
  {
    return;
  }

  shader.use();

  shader.setInt("pointLightsInUse", numPointLightsInUse);

  std::string position, constant, linear, quadratic, ambient, diffuse, specular;
  position = "pointLight[";
  position += which;
  position += "].";
  constant = linear = quadratic = ambient = diffuse = specular = position;
  position += "Position";
  constant += "Constant";
  linear += "Linear";
  quadratic += "Quadratic";
  ambient += "Ambient";
  diffuse += "Diffuse";
  specular += "Specular";

  shader.setVec3(position, light.Position);
  shader.setFloat(constant, light.Constant);
  shader.setFloat(linear, light.Linear);
  shader.setFloat(quadratic, light.Quadratic);
  shader.setVec3(ambient, light.Ambient);
  shader.setVec3(diffuse, light.Diffuse);
  shader.setVec3(specular, light.Specular);
}

static void setSpotLight(const SpotLight& light, const int which, const AAOGLShader& shader)
{
  if (which >= MAXSPOTLIGHTS)
  {
    return;
  }

  if (which >= numSpotLightsInUse)
  {
    return;
  }

  shader.use();

  shader.setInt("spotLightsInUse", numSpotLightsInUse);

  std::string position, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
  position = "spotLight[";
  position += which;
  position += "].";
  ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = position;
  position += "Position";
  constant += "Constant";
  cutoff += "CutOff";
  ocutoff += "OuterCutOff";
  direction += "Direction";
  linear += "Linear";
  quadrat += "Quadratic";
  ambient += "Ambient";
  diffuse += "Diffuse";
  specular += "Specular";

  shader.setVec3(position, light.Position);
  shader.setFloat(cutoff, light.CutOff);
  shader.setFloat(ocutoff, light.OuterCutOff);
  shader.setVec3(direction, light.Direction);
  shader.setFloat(linear, light.Linear);
  shader.setFloat(quadrat, light.Quadratic);
  shader.setVec3(ambient, light.Ambient);
  shader.setVec3(diffuse, light.Diffuse);
  shader.setVec3(specular, light.Specular);
}
