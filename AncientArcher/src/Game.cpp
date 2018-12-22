#include "Game.h"
#include "Controls.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Game::Game() {

  display = new Display();

  camera = new Camera();

  control = new Controls();   //default controls

  shader = new Shader("../AncientArcher/shaders/vertex.shader", "../AncientArcher/shaders/fragment.shader", "../AncientArcher/shaders/geometry.shader");

  player = new Player();   //default character

  /* load 16 textures */
  texBank.loadTexture("../AncientArcher/res/texture/00-pixelized_grass.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/01-pixelized_stone.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/02-pixelized_wood.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/03-pixelized_shrub.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/04-pixelized_portal.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/05-pixelized_darkstone.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/06-pixelized_water_droplet.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/07-pixelized_gravel.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/08-pixelized_water.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/09-pixelized_player.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/11-pixelized_darkwood.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/12-pickup_speedboost.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/13-pixelized_snow.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/14-maze_metal.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/15-pickup_attackboost.png", shader);

  //glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind VBO
  //glBindVertexArray(0);  //unbind VAO

  // only render the objects not line of sight blocked by other objects 
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
}

Game::~Game() {

  //glDeleteVertexArrays(1, &cubeVAO);
  //glDeleteBuffers(1, &cubeVBO);

  glfwTerminate();

}
