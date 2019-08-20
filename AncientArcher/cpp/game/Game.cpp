#include "Game.h"
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <Controls.h>
#include "../pckgs/randomLevel/RandomLevel.h"

extern Display g_display;
extern Controls g_controls;
extern bool g_projectionResize;

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

  g_controls.setKeyboard(keypress);   // sets the shared pointer for these
  g_controls.setMouse(mousepos);
  g_controls.setScroller(scrolling);

  world = new World();

  RandomLevel randomLevel;
  
  randomLevel.populateLayeredBlockGround(*world);
  randomLevel.populateBoundries(*world);
  randomLevel.populateMidSpheres(*world);
  randomLevel.popluateWalkwayPlanes(*world);

  modelShader = new Shader("../AncientArcher/resource/models/default.vert", "../AncientArcher/resource/models/default.frag");

  modelShader->use();
  glm::mat4 proj = world->getCamera()->getProjectionMatrix();
  modelShader->setMat4("projection", proj);

  demoModel = new Model("../AncientArcher/resource/models/nanosuit/nanosuit.obj");

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
  // this is a debug cam mover with no colliding - called every frame with deltaTime
  static const float FlyIncrement = 0.4f;
  static float flySpeed = 1.f;
  static float realVelocity = 0.f;
  static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
  static glm::vec3 moveFront = glm::vec3(*world->getCamera()->getFront());

  // set flyspeed when mouse wheel moves
  if (scrolling->yOffset > 0.1f) 
  {
    flySpeed += FlyIncrement;
    std::cout << "flySpeed: " << flySpeed << std::endl;
    scrolling->yOffset = 0;
  }
  if (scrolling->yOffset < -0.1f) 
  {
    flySpeed -= FlyIncrement;
    std::cout << "flySpeed: " << flySpeed << std::endl;
    scrolling->yOffset = 0;
  }

  // cap flyspeed
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

  // setting our velocity based on our current flyspeed
  realVelocity = dt * flySpeed;

  // process wasd
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

  // process page up/down for height
  if (keypress->pageup)
  {
    directionPlacement += WORLD_UP * realVelocity;
  }
  if (keypress->pagedown)
  {
    directionPlacement -= WORLD_UP * realVelocity;
  }

  // update projection matrix on shaders if window was resized
  if (g_projectionResize)
  {
    world->getCamera()->updateProjectionMatrix(modelShader);
    world->getCamera()->updateProjectionMatrix(world->getShader());
    g_projectionResize = !g_projectionResize;
  }

  // Set final new position this frame
  world->getCamera()->increasePosition(directionPlacement);                

  world->getLighting()->movePointLight(0, *world->getCamera()->getPosition(), world->getShader());  // debug point light stays at cam
  directionPlacement = glm::vec3(0.f, 0.f, 0.f);            // reset local variables for next frame processing
  moveFront = glm::vec3(*world->getCamera()->getFront());   // reset local variables
}