#include <Lighting.h>
#include <glad/glad.h>

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

void Lighting::setConstantLight(Shader* shader) {

  shader->use();
  // directional light
  shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  //shader->setVec3("dirLight.ambient", 0.15f, 0.15f, 0.15f);
  shader->setVec3("dirLight.ambient", 0.005f, 0.005f, 0.005f);
  //shader->setVec3("dirLight.diffuse", 0.15f, 0.15f, 0.15f);
  shader->setVec3("dirLight.diffuse", 0.005f, 0.005f, 0.005f);

  shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
}

void Lighting::addPointLight(glm::vec3 pos, Shader * shader) {

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
  //shader->setVec3(pLightambient.c_str(), 0.05f, 0.05f, 0.05f);
  //shader->setVec3(pLightambient.c_str(), 0.9f, 0.9f, 0.9f);
  //shader->setVec3(pLightambient.c_str(), 0.15f, 0.15f, 0.15f);
  shader->setVec3(pLightambient.c_str(), 0.35f, 0.35f, 0.35f);

  std::string pLightdiffuse = ss.str() + "diffuse";
  shader->setVec3(pLightdiffuse.c_str(), 0.8f, 0.8f, 0.8f);
  //shader->setVec3(pLightdiffuse.c_str(), 0.95f, 0.48f, 0.08f);
  //shader->setVec3(pLightdiffuse.c_str(), 0.65f, 0.30f, 0.0f);
  //shader->setVec3(pLightdiffuse.c_str(), 0.75f, 0.35f, 0.005f);

  std::string pLightspecular = ss.str() + "specular";
  shader->setVec3(pLightspecular, 1.0f, 1.0f, 1.0f);

  std::string pLightconstant = ss.str() + "constant";
  shader->setFloat(pLightconstant, 1.0f);
  //shader->setFloat(pLightconstant, 0.5f);

  std::string pLightlinear = ss.str() + "linear";
  shader->setFloat(pLightlinear, 0.36f);
  //shader->setFloat(pLightlinear, 0.09f);
  //shader->setFloat(pLightlinear, 0.01f);
  //shader->setFloat(pLightlinear, 0.0002f);

  std::string pLightquadratic = ss.str() + "quadratic";
  //shader->setFloat(pLightquadratic, 0.064f);
  //shader->setFloat(pLightquadratic, 0.032f);
  //shader->setFloat(pLightquadratic, 0.016f);
  //shader->setFloat(pLightquadratic, 0.01f);
  shader->setFloat(pLightquadratic, 0.002f);

  shader->setInt("numPointLights", (int)i + 1);

  pointLights.push_back(pos);

  std::cout << "point Light " << ss.str() << " added!\n";

  _currentPointLights = i + 1;

}

void Lighting::movePointLight(int lightnum, glm::vec3 newpos, Shader * shader) {
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
