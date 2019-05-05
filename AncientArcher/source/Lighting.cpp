#include <glad/glad.h>

#include "Lighting.h"
#include "globals.h"

#include <string>
#include <sstream>
#include <iostream>

Lighting::Lighting() {}

Lighting::~Lighting() {}

void Lighting::setConstantLight() {

  // directional light
  texBankShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  texBankShader.setVec3("dirLight.ambient", 0.95f, 0.95f, 0.95f);
  //texBankShader.setVec3("dirLight.diffuse", 0.15f, 0.15f, 0.15f);    
  texBankShader.setVec3("dirLight.diffuse", 0.95f, 0.95f, 0.95f);
  texBankShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

}

void Lighting::addPointLight(glm::vec3 pos) {
 
  std::size_t i = pointLights.size();
  
  if (i >= MAXPOINTLIGHTS) {   // make sure we are not going past what our texBankShader allows
    std::cout << MAXPOINTLIGHTS << " reached, no more can be added.\n";
    return;
  }
  
  std::stringstream ss("pointLight[", std::ios_base::app | std::ios_base::out);
  ss << i << "].";

  texBankShader.use();

  std::string pLightposition = ss.str() + "position";   // pointLight[i].position
  texBankShader.setVec3(pLightposition.c_str(), pos);

  std::string pLightambient = ss.str() + "ambient";   // pointLight[i].ambient
  //texBankShader.setVec3(pLightambient.c_str(), 0.05f, 0.05f, 0.05f);
  //texBankShader.setVec3(pLightambient.c_str(), 0.9f, 0.9f, 0.9f);
  //texBankShader.setVec3(pLightambient.c_str(), 0.15f, 0.15f, 0.15f);
  texBankShader.setVec3(pLightambient.c_str(), 0.35f, 0.35f, 0.35f);

  std::string pLightdiffuse = ss.str() + "diffuse";
  texBankShader.setVec3(pLightdiffuse.c_str(), 0.8f, 0.8f, 0.8f);
  //texBankShader.setVec3(pLightdiffuse.c_str(), 0.95f, 0.48f, 0.08f);
  //texBankShader.setVec3(pLightdiffuse.c_str(), 0.65f, 0.30f, 0.0f);
  //texBankShader.setVec3(pLightdiffuse.c_str(), 0.75f, 0.35f, 0.005f);

  std::string pLightspecular = ss.str() + "specular";
  texBankShader.setVec3(pLightspecular, 1.0f, 1.0f, 1.0f);

  std::string pLightconstant = ss.str() + "constant";
  texBankShader.setFloat(pLightconstant, 1.0f);
  //texBankShader.setFloat(pLightconstant, 0.5f);

  std::string pLightlinear = ss.str() + "linear";
  //texBankShader.setFloat(pLightlinear, 0.36);
  //texBankShader.setFloat(pLightlinear, 0.09);
  texBankShader.setFloat(pLightlinear, 0.01f);
  //texBankShader.setFloat(pLightlinear, 0.0002f);

  std::string pLightquadratic = ss.str() + "quadratic";
  //texBankShader.setFloat(pLightquadratic, 0.064f);

  //texBankShader.setFloat(pLightquadratic, 0.032f);
  texBankShader.setFloat(pLightquadratic, 0.016f);
  //texBankShader.setFloat(pLightquadratic, 0.002);

  texBankShader.setInt("numPointLights", (int)i + 1);

  pointLights.push_back(pos);

  std::cout << "point Light " << ss.str() << " added!\n";

}

void Lighting::movePointLight(int lightnum, glm::vec3 newpos) {
  texBankShader.setVec3("pointLight[0].position", newpos);
}


