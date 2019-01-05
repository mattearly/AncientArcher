#include <glad/glad.h>

#include "Lighting.h"
#include "../shaders/Shader.h"

#include <string>
#include <sstream>
#include <iostream>

extern Shader shader;

Lighting::Lighting() {

}

Lighting::~Lighting() {}

//void Lighting::draw() {
//
//}

void Lighting::setConstantLight() {

  //// directional light
  shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

  // point light 1
  //shader.setVec3("pointLight.position", 10.0f, 10.0f, 10.0f);  //currently on player position
  //shader.setVec3("pointLight.ambient", 0.05f, 0.05f, 0.05f);
  //shader.setVec3("pointLight.diffuse", 0.8f, 0.8f, 0.8f);
  //shader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
  //shader.setFloat("pointLight.constant", 1.0f);
  //shader.setFloat("pointLight.linear", 0.09);
  //shader.setFloat("pointLight.quadratic", 0.032);
}

void Lighting::addPointLight(glm::vec3 pos) {
 
  int i = pointLights.size();
  
  if (i >= MAXPOINTLIGHTS) {   // make sure we are not going past what our shader allows
    std::cout << MAXPOINTLIGHTS << " reached, no more can be added.\n";
    return;
  }
  
  std::stringstream ss("pointLight[", std::ios_base::app | std::ios_base::out);
  ss << i << "].";

  shader.use();

  std::string pLightposition = ss.str() + "position";   // pointLight[i].position
  shader.setVec3(pLightposition.c_str(), pos);

  std::string pLightambient = ss.str() + "ambient";   // pointLight[i].ambient
  shader.setVec3(pLightambient.c_str(), 0.05f, 0.05f, 0.05f);

  std::string pLightdiffuse = ss.str() + "diffuse";
  shader.setVec3(pLightdiffuse.c_str(), 0.8f, 0.8f, 0.8f);

  std::string pLightspecular = ss.str() + "specular";
  shader.setVec3(pLightspecular, 1.0f, 1.0f, 1.0f);

  std::string pLightconstant = ss.str() + "constant";
  shader.setFloat(pLightconstant, 1.0f);

  std::string pLightlinear = ss.str() + "linear";
  shader.setFloat(pLightlinear, 0.09);

  std::string pLightquadratic = ss.str() + "quadratic";
  shader.setFloat(pLightquadratic, 0.032);

  shader.setInt("numPointLights", i + 1);

  pointLights.push_back(pos);

  std::cout << "point Light " << ss.str() << " added!\n";

}


