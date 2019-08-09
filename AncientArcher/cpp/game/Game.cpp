#include "Game.h"
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <Controls.h>
#include "../pckgs/randomLevel/RandomLevel.h"

extern Display g_display;
extern Controls g_controls;

Game::Game()
{
  int __textures_allowed = 0, __totalTexturesAllowed = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

  keypress = std::make_shared<keys>();
  mousepos = std::make_shared<mouse>();
  scrolling = std::make_shared<scroll>();

  g_controls.setKeyboard(keypress);
  g_controls.setMouse(mousepos);
  g_controls.setScroller(scrolling);

  world = new World();

  RandomLevel randomLevel;
  
  randomLevel.populateLayeredBlockGround(*world);
  randomLevel.populateBoundries(*world);

  //player = new FirstPersonPlayer(world->getSharedCamera(), world->getSharedShader());

  // LOAD SKYBOX 
  std::vector<std::string> skyboxFiles =
  {
    "../AncientArcher/cpp/pckgs/skybox/stars/right.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/left.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/top.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/bottom.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/front.png",
    "../AncientArcher/cpp/pckgs/skybox/stars/back.png"
  };
  sky = new Skybox(world->getSharedCamera(), skyboxFiles);

  //sky = new SkyboxRenderer(world->getSharedCamera());

  world->getLighting()->addPointLight(*world->getCamera()->getPosition(), world->getShader());   //debug point light
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////HELPER FUNCTIONS//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void Game::moveCamHelper(float dt)
{
  // this is a debug cam mover with no colliding
  static const float FlyIncrement = 0.4f;
  static float flySpeed = 1.f;
  static float realVelocity = 0.f;
  static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
  static glm::vec3 moveFront = glm::vec3(*world->getCamera()->getFront());

  if (scrolling->yOffset > 0.1f) {

    flySpeed += FlyIncrement;
    std::cout << "flySpeed: " << flySpeed << std::endl;
    scrolling->yOffset = 0;
  }

  if (scrolling->yOffset < -0.1f) {

    flySpeed -= FlyIncrement;
    std::cout << "flySpeed: " << flySpeed << std::endl;
    scrolling->yOffset = 0;

  }
  if (flySpeed >= 10.f)
  {
    flySpeed = 9.999999f;
    std::cout << "flySpeed: " << flySpeed << std::endl;

  }
  if (flySpeed <= 1.f)
  {
    flySpeed = 1.000001f;
    std::cout << "flySpeed: " << flySpeed << std::endl;

  }

  realVelocity = dt * flySpeed;

  if (keypress->w)
  {
    directionPlacement += moveFront * realVelocity;
  }

  if (keypress->s)
  {
    directionPlacement -= moveFront * realVelocity;
  }

  if (keypress->a)
  {
    directionPlacement -= *world->getCamera()->getRight() * realVelocity;
  }

  if (keypress->d)
  {
    directionPlacement += *world->getCamera()->getRight() * realVelocity;
  }

  world->getCamera()->increasePosition(directionPlacement);                // Set final new position

  world->getLighting()->movePointLight(0, *world->getCamera()->getPosition(), world->getShader());  // debug point light stays at cam
  directionPlacement = glm::vec3(0.f, 0.f, 0.f);            // reset local variables
  moveFront = glm::vec3(*world->getCamera()->getFront());   // reset local variables
}