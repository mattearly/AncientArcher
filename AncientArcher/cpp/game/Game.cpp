#include "Game.h"
#include <glm/glm.hpp>
#include <iostream>
#include <Display.h>
#include <Controls.h>
#include "../pckgs/sceneLoader/LevelLoader.h"

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

  _keypress = std::make_shared<keys>();
  _mousepos = std::make_shared<mouse>();
  _scrolling = std::make_shared<scroll>();
  _camera = std::make_shared<Camera>(glm::vec3(0.f, 5.0f, 0.f), -90.f, -45.0f, 80.f);
  _shader = std::make_shared<Shader>("../AncientArcher/resource/world/primative.vert", "../AncientArcher/resource/world/primative.frag");
  _shader->use();
  glm::mat4 proj = _camera->getProjectionMatrix();
  _shader->setMat4("projection", proj);
  _lighting = std::make_shared<Lighting>();
  _lighting->setConstantLight(_shader.get());

  g_controls.setKeyboard(_keypress);   // sets the shared pointer for these
  g_controls.setMouse(_mousepos);
  g_controls.setScroller(_scrolling);

  std::vector<std::string> skyboxFiles =
  {
    "../AncientArcher/resource/skybox/stars/right.png",
    "../AncientArcher/resource/skybox/stars/left.png",
    "../AncientArcher/resource/skybox/stars/top.png",
    "../AncientArcher/resource/skybox/stars/bottom.png",
    "../AncientArcher/resource/skybox/stars/front.png",
    "../AncientArcher/resource/skybox/stars/back.png"
  };
  scene.setupSkybox(_camera, skyboxFiles);

  scene.setShader(_shader);
  scene.setCamera(_camera);

  LevelLoader levelLoader;
  levelLoader.loadDemoLevel(scene);


  _lighting->addPointLight(*_camera->getPosition(), _shader.get()); // debug point light
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
  static glm::vec3 moveFront = glm::vec3(*_camera->getFront());

  // set flyspeed when mouse wheel moves
  if (_scrolling->yOffset > 0.1f)
  {
    flySpeed += FlyIncrement;
    std::cout << "flySpeed: " << flySpeed << std::endl;
    _scrolling->yOffset = 0;
  }
  if (_scrolling->yOffset < -0.1f)
  {
    flySpeed -= FlyIncrement;
    std::cout << "flySpeed: " << flySpeed << std::endl;
    _scrolling->yOffset = 0;
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
  if (_keypress->w)
  {
    directionPlacement += moveFront * realVelocity;
  }
  if (_keypress->s)
  {
    directionPlacement -= moveFront * realVelocity;
  }
  if (_keypress->a)
  {
    directionPlacement -= *_camera->getRight() * realVelocity;
  }
  if (_keypress->d)
  {
    directionPlacement += *_camera->getRight() * realVelocity;
  }

  // process page up/down for height
  if (_keypress->pageup)
  {
    directionPlacement += WORLD_UP * realVelocity;
  }
  if (_keypress->pagedown)
  {
    directionPlacement -= WORLD_UP * realVelocity;
  }

  // update projection matrix on shaders if window was resized
  if (g_projectionResize)
  {
    _camera->updateProjectionMatrix(_shader.get());
    g_projectionResize = !g_projectionResize;
  }

  // Set final new position this frame
  _camera->increasePosition(directionPlacement);

  _lighting->movePointLight(0, *_camera->getPosition(), _shader.get());  // debug point light stays at cam
  directionPlacement = glm::vec3(0.f, 0.f, 0.f);            // reset local variables for next frame processing
  moveFront = glm::vec3(*_camera->getFront());   // reset local variables
}