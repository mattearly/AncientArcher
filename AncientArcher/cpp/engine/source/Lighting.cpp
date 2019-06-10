#include <Lighting.h>
#include <glad/glad.h>

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

/**
 * Constant Directional Light. Sets it on the shader with the Variables as in code.
 * Call this again after making updates to the lighting to apply the changes.
 * @param shader   Shader to have the variables set in.
 */
void Lighting::setConstantLight(Shader* shader) {

  shader->use();
  shader->setVec3("dirLight.direction", direction);
  shader->setVec3("dirLight.ambient", ambient);
  shader->setVec3("dirLight.diffuse", diffuse);
  shader->setVec3("dirLight.specular", specular);
}

void Lighting::updateConstantLightDirection(glm::vec3 changedVar)
{
  clampVec3BetweenNeg1and1(changedVar);
  direction = changedVar;
}

void Lighting::updateConstantLightAmbient(glm::vec3 changedVar)
{
  clampVec3Between0and1(changedVar);
  ambient = changedVar;
}

void Lighting::updateConstantLightDiffuse(glm::vec3 changedVar)
{
  clampVec3Between0and1(changedVar);
  diffuse = changedVar;
}

void Lighting::updateConstantLightSpecular(glm::vec3 changedVar)
{
  clampVec3Between0and1(changedVar);
  specular = changedVar;
}

void Lighting::addPointLight(glm::vec3 pos, Shader* shader) {

  std::size_t i = pointLights.size();

  if (i >= MAXPOINTLIGHTS) {   // make sure we are not going past what our texBankShader allows
    std::cout << MAXPOINTLIGHTS << " reached, no more can be added.\n";
    return;
  }

  std::stringstream ss("pointLight[", std::ios_base::app | std::ios_base::out);
  ss << i << "].";

  shader->use();

  std::string pLightposition = ss.str() + "position";   // pointLight[i].position
  shader->setVec3(pLightposition.c_str(), pos);

  std::string pLightambient = ss.str() + "ambient";   // pointLight[i].ambient
  shader->setVec3(pLightambient.c_str(), 0.05f, 0.05f, 0.05f);  // default
  //shader->setVec3(pLightambient.c_str(), 0.9f, 0.9f, 0.9f);
  //shader->setVec3(pLightambient.c_str(), 0.15f, 0.15f, 0.15f);
  //shader->setVec3(pLightambient.c_str(), 0.35f, 0.35f, 0.35f);

  std::string pLightdiffuse = ss.str() + "diffuse";
  //shader->setVec3(pLightdiffuse.c_str(), 0.5f, 0.5f, 0.55f);
  shader->setVec3(pLightdiffuse.c_str(), 0.8f, 0.8f, 0.8f); // default
  //shader->setVec3(pLightdiffuse.c_str(), 0.95f, 0.48f, 0.08f);
  //shader->setVec3(pLightdiffuse.c_str(), 0.65f, 0.30f, 0.0f);
  //shader->setVec3(pLightdiffuse.c_str(), 0.75f, 0.35f, 0.005f);
  //shader->setVec3(pLightdiffuse.c_str(), 0.15f, 0.05f, 0.10f);

  std::string pLightspecular = ss.str() + "specular";
  shader->setVec3(pLightspecular, 1.f, 1.f, 1.f); // default
  //shader->setVec3(pLightspecular, .4f, .4f, .4f);
  //shader->setVec3(pLightspecular, .05f, .05f, .05f);
  //shader->setVec3(pLightspecular, 256.f, 256.f, 256.f);

  std::string pLightconstant = ss.str() + "constant";
  shader->setFloat(pLightconstant, 1.0f);  // default
  //shader->setFloat(pLightconstant, 0.5f);

  std::string pLightlinear = ss.str() + "linear";
  //shader->setFloat(pLightlinear, 0.36f);
  shader->setFloat(pLightlinear, 0.09f);  // default
  //shader->setFloat(pLightlinear, 0.01f);
  //shader->setFloat(pLightlinear, 0.0002f);

  std::string pLightquadratic = ss.str() + "quadratic";
  //shader->setFloat(pLightquadratic, 0.064f);
  shader->setFloat(pLightquadratic, 0.032f);  // default
  //shader->setFloat(pLightquadratic, 0.016f);
  //shader->setFloat(pLightquadratic, 0.01f);
  //shader->setFloat(pLightquadratic, 0.002f);

  shader->setInt("numPointLights", (int)i + 1);

  pointLights.push_back(pos);

  std::cout << "point Light " << ss.str() << " added!\n";

  _currentPointLights = i + 1;

}

void Lighting::movePointLight(int lightnum, glm::vec3 newpos, Shader* shader) {
  if (lightnum < _currentPointLights)
  {
    shader->use();
    switch (lightnum) {
    case 0:
      shader->setVec3("pointLight[0].position", newpos);
      break;
    case 1:
      shader->setVec3("pointLight[1].position", newpos);
      break;
    case 2:
      shader->setVec3("pointLight[2].position", newpos);
      break;
    case 3:
      shader->setVec3("pointLight[3].position", newpos);
      break;
    }
  }
  else
  {
    throw std::runtime_error("Attempting to move point light that does not exist");
  }

}

void Lighting::clampVec3Between0and1(glm::vec3& changedVar)
{
  if (changedVar.x > 1.f) changedVar.x = 1.f;
  else if (changedVar.x < 0.f) changedVar.x = 0.f;
  if (changedVar.y > 1.f) changedVar.y = 1.f;
  else if (changedVar.y < 0.f) changedVar.y = 0.f;
  if (changedVar.z > 1.f) changedVar.z = 1.f;
  else if (changedVar.z < 0.f) changedVar.z = 0.f;
}

void Lighting::clampVec3BetweenNeg1and1(glm::vec3& changedVar)
{
  if (changedVar.x > 1.f) changedVar.x = 1.f;
  else if (changedVar.x < -1.f) changedVar.x = -1.f;
  if (changedVar.y > 1.f) changedVar.y = 1.f;
  else if (changedVar.y < -1.f) changedVar.y = -1.f;
  if (changedVar.z > 1.f) changedVar.z = 1.f;
  else if (changedVar.z < -1.f) changedVar.z = -1.f;
}
