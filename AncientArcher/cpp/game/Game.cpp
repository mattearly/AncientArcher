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
  //int __textures_allowed = 0, __totalTexturesAllowed = 0;
  //glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &__textures_allowed);
  //glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &__totalTexturesAllowed);
  //std::cout << "//--GRAPHIC CARD INFO--//\nMax textures per shader:  " << __textures_allowed << "\n";
  //std::cout << "Max total textures:  " << __totalTexturesAllowed << "\n";

  _keypress  = std::make_shared<keys>();
  _mousepos  = std::make_shared<mouse>();
  _scrolling = std::make_shared<scroll>();
  _camera    = std::make_shared<Camera>(glm::vec3(0.f, 5.0f, 0.f), -90.f, -45.0f, 80.f);
  _lighting = std::make_shared<Lighting>();

  glm::mat4 proj = _camera->getProjectionMatrix();

  _shader = std::make_shared<Shader>(
    "../AncientArcher/resource/world/primative.vert", 
    "../AncientArcher/resource/world/primative.frag");
  //_shader->use();
  _shader->setMat4("projection", proj);

  _animModelShader = std::make_shared<Shader>("../AncientArcher/resource/world/animModel.vert", "../AncientArcher/resource/world/primative.frag");
  //_animModelShader->use();
  _animModelShader->setMat4("projection", proj);

  _lighting->setConstantLight(_shader.get());
  _lighting->setConstantLight(_animModelShader.get());

  g_controls.setKeyboard(_keypress);   // sets the shared pointer for these
  g_controls.setMouse(_mousepos);
  g_controls.setScroller(_scrolling);

  std::vector<std::string> skyboxFiles =
  {
    "../AncientArcher/resource/skybox/default/right.jpg",
    "../AncientArcher/resource/skybox/default/left.jpg",
    "../AncientArcher/resource/skybox/default/top.jpg",
    "../AncientArcher/resource/skybox/default/bottom.jpg",
    "../AncientArcher/resource/skybox/default/front.jpg",
    "../AncientArcher/resource/skybox/default/back.jpg"
  };
  scene.setupSkybox(_camera, skyboxFiles);

  scene.setShader(_shader);
  //scene.setAnimShader(_animModelShader);
  scene.setAnimShader(_shader);
  scene.setCamera(_camera);

  //LevelLoader::getLevelLoader()->loadDemoLevel(scene);
  LevelLoader::getLevelLoader()->loadDemoLevel5(scene);  // testwuson.X file
  
  _lighting->addPointLight(*_camera->getPosition(), _shader.get());          // debug point light
  _lighting->addPointLight(*_camera->getPosition(), _animModelShader.get()); // debug point light
}


void Game::moveCamHelper(float dt)
{
  // this is a debug cam mover with no colliding - called every frame with deltaTime
  static const float FlyIncrement = 0.4f;
  static float flySpeed = 5.f;
  static float prevFlySpeed = 0.f;
  static float realVelocity = 0.f;
  static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
  static glm::vec3 moveFront = glm::vec3(*_camera->getFront());

  // set flyspeed when mouse wheel moves
  if (_scrolling->yOffset > 0.1f)
  {
    flySpeed += FlyIncrement;
    _scrolling->yOffset = 0;
  }
  if (_scrolling->yOffset < -0.1f)
  {
    flySpeed -= FlyIncrement;
    _scrolling->yOffset = 0;
  }

  // cap flyspeed
  if (flySpeed >= 10.f)
  {
    flySpeed = 10.f;

  }
  if (flySpeed <= 1.f)
  {
    flySpeed = 1.000001f;
  }

  if (flySpeed != prevFlySpeed)  // show flySpeed in debug console if it changed since last frame.
  {
    std::cout << "flySpeed: " << flySpeed << std::endl;
    prevFlySpeed = flySpeed;
  }

  // setting our velocity based on our current flyspeed
  realVelocity = dt * flySpeed;

  // process wasd
  if (_keypress->w)
  {
    // debug show location on W press
    std::cout << "Loc(x,z,y): (" 
      << _camera->getPosition()->x << "," 
      << _camera->getPosition()->z << ","
      << _camera->getPosition()->y << ")\n";

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

  // process going up and down
  if (!_keypress->leftShift && _keypress->spacebar)  // spacebar goes up
  {
    directionPlacement += WORLD_UP * realVelocity;
  }
  if (_keypress->leftShift && _keypress->spacebar)   // shift + spacebar goes down
  {
    directionPlacement -= WORLD_UP * realVelocity;
  }

  // check if level switch button is pressed ( TEST )
  if (_keypress->_1)
  {
    LevelLoader::getLevelLoader()->loadDemoLevel(scene);
  }

  if (_keypress->_2)
  {
    LevelLoader::getLevelLoader()->loadDemoLevel2(scene);
  }

  if (_keypress->_3)
  {
    LevelLoader::getLevelLoader()->loadDemoLevel3(scene);
  }  
  
  if (_keypress->_4)
  {
    LevelLoader::getLevelLoader()->loadDemoLevel4(scene);
  }

  if (_keypress->_5)
  {
    LevelLoader::getLevelLoader()->loadDemoLevel5(scene);
  }

  // update projection matrix on shaders if window was resized
  if (g_projectionResize)
  {
    _camera->updateProjectionMatrix(_shader.get());
    _camera->updateProjectionMatrix(_animModelShader.get());
    g_projectionResize = !g_projectionResize;
  }

  // Set final new position this frame
  _camera->increasePosition(directionPlacement);
  _lighting->movePointLight(0, *_camera->getPosition(), _shader.get());  // debug point light stays at cam
  _lighting->movePointLight(0, *_camera->getPosition(), _animModelShader.get());  // debug point light stays at cam

  // reset local variables for next frame processing
  directionPlacement = glm::vec3(0.f, 0.f, 0.f);
  moveFront = glm::vec3(*_camera->getFront());

}