#include "Game.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../globals.h"
#include "../controls/Controls.h"
#include "../Constraints.h"
#include <iostream>

Display display;
Shader shader("../AncientArcher/src/shaders/vertex.shader", "../AncientArcher/src/shaders/fragment.shader");
Camera camera;
Controls controls;
Pickups pickups;
Lighting lighting;
TextureBank texBank;
DiffuseTexture diffuseTex;
std::vector<Entity> entities;


Game::Game() {

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

  lighting.setConstantLight();


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

  int width = round(world_width + 1);

  for (int i = 0; i < 3; i++) {
    Entity floor(
      // LOCATION 
      glm::vec3(i * width, 0.0f, width / 2),
      // SIZE
      glm::vec3(width, 0.5f, width),
      // TEXTURE ID
      i,
      // COLLISION?
      false
    );
    entities.push_back(floor);
  }

  for (int i = 0; i < 3; i++) {
    Entity floor(
      // LOCATION 
      glm::vec3(i * width, 0.0f, width * 1.5 - .5),
      // SIZE
      glm::vec3(width, 0.5f, width),
      // TEXTURE ID
      i+2,
      // COLLISION?
      false
    );
    entities.push_back(floor);
  }  

  for (int i = 0; i < 3; i++) {
    Entity floor(
      // LOCATION 
      glm::vec3(i * width, 0.0f, width * 2.5 - .5f),
      // SIZE
      glm::vec3(width, 0.5f, width),
      // TEXTURE ID
      i + 3,
      // COLLISION?
      false
    );
    entities.push_back(floor);
  }
  ////floor
  //Entity entfloor1(glm::vec3(width / 2, 0.0f, width / 2), glm::vec3(width, 0.001f, width), 1, false);
  //entities.push_back(entfloor1); //main floor

  //Entity entfloor2(glm::vec3(-width / 2 - 1, 0.0f, width / 2 - 1), glm::vec3(width, 0.001f, width), 4, false);
  //entities.push_back(entfloor2);        
  //Entity entfloor3(glm::vec3(width / 2  - 1, 0.0f, -width / 2 - 1), glm::vec3(width, 0.001f, width), 3, false);
  //entities.push_back(entfloor3);        
  //Entity entfloor4(glm::vec3(-width / 2 - 1, 0.0f, -width / 2 - 1), glm::vec3(width, 0.001f, width), 11, false);
  //entities.push_back(entfloor4);
  //Entity entfloor5(glm::vec3(width / 2, 0.0f, width / 2), glm::vec3(width, 0.001f, width), 6, false);
  //entities.push_back(entfloor5);

  //walls
  //Entity ent1(glm::vec3(width / 2, 2.0f, -1.f), glm::vec3(width, 4.0f, 1.0), 10, true);
  //entities.push_back(ent1);

  //Entity ent2(glm::vec3(-1.f, 2.0f, width / 2), glm::vec3(1.0f, 4.0f, width), 2, true);
  //entities.push_back(ent2);

  //Entity ent3(glm::vec3(width, 2.0f, width / 2), glm::vec3(1.0f, 4.0f, width), 10, true);
  //entities.push_back(ent3);

  //Entity ent4(glm::vec3(width / 2, 2.0f, width), glm::vec3(width, 4.0f, 1), 2, true);
  //entities.push_back(ent4);

  // A test collidable block
  Entity ent5(glm::vec3(6.f, 1.0f, 6.f), glm::vec3(2.f, 2.f, 2.f), 6, true);
  entities.push_back(ent5);

}

Game::~Game() {

}
