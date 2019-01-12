#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../camera/Camera.h"
#include "../displayManager/Display.h"
#include "../controls/Controls.h"
#include "../shaders/Shader.h"
#include "../util/TextureBank.h"
#include "../util/DiffuseTexture.h"
#include "../Constraints.h"
#include "../lighting/Lighting.h"

Display display;
Shader shader("../AncientArcher/src/shaders/vertex.shader", "../AncientArcher/src/shaders/fragment.shader");
Camera camera;
Controls controls;
Pickups pickups;
TextureBank texBank;
DiffuseTexture diffuseTex;
Lighting lighting;

Game::Game() {

  //display = new Display();

  //camera = new Camera();

  //controls = new Controls();   //default controls

  //shader = new Shader("../AncientArcher/shaders/vertex.shader", "../AncientArcher/shaders/fragment.shader", "../AncientArcher/shaders/geometry.shader");

  player = new Player();   //default character

  /* load 16 textures */
  texBank.loadTexture("../AncientArcher/res/texture/00-pixelized_grass.png");
  texBank.loadTexture("../AncientArcher/res/texture/01.png");
  texBank.loadTexture("../AncientArcher/res/texture/02-pixelized_wood.png");
  texBank.loadTexture("../AncientArcher/res/texture/03-pixelized_shrub.png");
  texBank.loadTexture("../AncientArcher/res/texture/04-pixelized_portal.png");
  texBank.loadTexture("../AncientArcher/res/texture/05-pixelized_darkstone.png");
  texBank.loadTexture("../AncientArcher/res/texture/06-pixelized_water_droplet.png");
  texBank.loadTexture("../AncientArcher/res/texture/07-pixelized_gravel.png");
  texBank.loadTexture("../AncientArcher/res/texture/08-pixelized_water.png");
  texBank.loadTexture("../AncientArcher/res/texture/09-pixelized_player.png");
  texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png");
  texBank.loadTexture("../AncientArcher/res/texture/11-pixelized_darkwood.png");
  texBank.loadTexture("../AncientArcher/res/texture/12-pickup_speedboost.png");
  texBank.loadTexture("../AncientArcher/res/texture/13-pixelized_snow.png");
  texBank.loadTexture("../AncientArcher/res/texture/14-maze_metal.png");
  texBank.loadTexture("../AncientArcher/res/texture/15-pickup_attackboost.png");

  diffuseTex.loadDiffuseTexture("../AncientArcher/res/specular/none.png");

  // only render the objects not line of sight blocked by other objects 
  //glEnable(GL_CULL_FACE);

  //lighting.setConstantLight();


  // demo point lights
  //lighting.addPointLight(glm::vec3(1.5f, 4.0f, 1.5f));
  //lighting.addPointLight(glm::vec3(1.5f, 4.0f, 38.5f));
  //lighting.addPointLight(glm::vec3(38.5f, 4.0f, 1.5f));
  //lighting.addPointLight(glm::vec3(38.5f, 4.0f, 38.5f));
  lighting.addPointLight(*camera.getPosition());

  renderer.enableGLDepthTest();

  camera.updateProjectionMatrix();

  //shader.setInt("specnum", 0);

  // spotLight
//  shader.setVec3("spotLight.position", *(camera.getPosition()));
//  shader.setVec3("spotLight.direction", *(camera.getFront()));
//  shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//  shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//  shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//  shader.setFloat("spotLight.constant", 1.0f);
//  shader.setFloat("spotLight.linear", 0.09);
//  shader.setFloat("spotLight.quadratic", 0.032);
//  shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//  shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

Game::~Game() {

}
