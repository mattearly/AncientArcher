#pragma once
#include <glm/glm.hpp>
#include <sstream>

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

// single directional lights
static void setDirectionalLight(const DirectionalLight& light, const AAOGLShader& shader)
{
  shader.use();

  std::string direction, ambient, diffuse, specular;
  ambient = diffuse = specular = direction = "directionalLight.";
  
  direction += "Direction";
  ambient +=   "Ambient";
  diffuse +=   "Diffuse";
  specular +=  "Specular";

  shader.setVec3(direction, light.Direction);
  shader.setVec3(ambient,   light.Ambient);
  shader.setVec3(diffuse,   light.Diffuse);
  shader.setVec3(specular,  light.Specular);
}

// multi lights
static void setPointLight(const PointLight& light, const int which, const AAOGLShader& shader)
{
  if (which >= MAXPOINTLIGHTS)
  {
    std::cout << "couldn't set PointLight light, too many\n";
    return;
  }
  
  if (which >= numPointLightsInUse)
  {
    std::cout << "couldn't set PointLight light, not enough in use\n";
    return;
  }

  shader.use();

  shader.setInt("pointLightsInUse", numPointLightsInUse);

  std::string position, constant, linear, quadratic, ambient, diffuse, specular;  
  constant = linear = quadratic = ambient = diffuse = specular = position = "pointLight[";
  
  std::stringstream ss;
  ss << which;

  position  += ss.str();
  constant  += ss.str();
  linear    += ss.str();
  quadratic += ss.str();
  ambient   += ss.str();
  diffuse   += ss.str();
  specular  += ss.str();

  position  += "].";
  constant  += "].";
  linear    += "].";
  quadratic += "].";
  ambient   += "].";
  diffuse   += "].";
  specular  += "].";
 
  position  += "Position";
  constant  += "Constant";
  linear    += "Linear";
  quadratic += "Quadratic";
  ambient   += "Ambient";
  diffuse   += "Diffuse";
  specular  += "Specular";

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
    std::cout << "couldn't set SpotLight light, too many\n";
    return;
  }

  if (which >= numSpotLightsInUse)
  {
    std::cout << "couldn't set SpotLight light, not enough in use\n";
    return;
  }

  shader.use();

  shader.setInt("spotLightsInUse", numSpotLightsInUse);

  std::string position, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
  ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = position = "spotLight[";

  std::stringstream ss;
  ss << which;

  position  += ss.str();
  constant  += ss.str();
  cutoff    += ss.str();
  ocutoff   += ss.str();
  direction += ss.str();
  linear    += ss.str();
  quadrat   += ss.str();
  ambient   += ss.str();
  diffuse   += ss.str();
  specular  += ss.str();

  position  += "].";
  constant  += "].";
  cutoff    += "].";
  ocutoff   += "].";
  direction += "].";
  linear    += "].";
  quadrat   += "].";
  ambient   += "].";
  diffuse   += "].";
  specular  += "].";

  position  += "Position";
  constant  += "Constant";
  cutoff    += "CutOff";
  ocutoff   += "OuterCutOff";
  direction += "Direction";
  linear    += "Linear";
  quadrat   += "Quadratic";
  ambient   += "Ambient";
  diffuse   += "Diffuse";
  specular  += "Specular";

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
