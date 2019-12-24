#pragma once
#include "../engine/AAEngine.h"
#include "../engine/AAGameObject.h"
#include "../engine/AAShaderManager.h"
#include "../engine/AADisplay.h"
#include "dry/addGodMovement.h"
#include "dry/addPreferedDefaults.h"
#include "../engine/AAOGLGraphics.h"
#include "dry/addSkybox.h"
void testEngineFuncsDemo_2()
{
  AAEngine engine;

  addSkybox(engine, "nordic");

  addGodMovement(engine);

  addPreferredDefaults(engine);

  // add shaders to use
  static AAShaderManager shaderMan;
  static int noLightShader = shaderMan.addShader("../AncientArcher/shader/vert_textured.glsl", "../AncientArcher/shader/frag_noLight.glsl");
   
  // add object to draw
  static AAGameObject mountainGround = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model10, true);
  mountainGround.translate(glm::vec3(0.f, -20.f, 0));
  static AAGameObject streetArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model9, true);
  streetArea.translate(glm::vec3(220.f, -20.f, 0));
  static AAGameObject waterArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model7, true);
  waterArea.translate(glm::vec3(-220.f, -20.f, 0));  
  static AAGameObject charredArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model11, true);
  charredArea.translate(glm::vec3(0.f, -20.f, 220.f));
  static AAGameObject dirtArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model12, true);
  dirtArea.translate(glm::vec3(0.f, -20.f, -220.f));
  static AAGameObject brickArea = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model13, true);
  brickArea.translate(glm::vec3(0.f, -200.f, -220.f));
  brickArea.scale(glm::vec3(20.f, 0.f, 20.f));

  auto drawObjects = []()
  {
    mountainGround.draw(shaderMan.getShader(noLightShader));
    streetArea.draw(shaderMan.getShader(noLightShader));
    waterArea.draw(shaderMan.getShader(noLightShader));
    charredArea.draw(shaderMan.getShader(noLightShader));
    dirtArea.draw(shaderMan.getShader(noLightShader));
    brickArea.draw(shaderMan.getShader(noLightShader));
  };
  engine.addToOnRender(drawObjects);

  auto updateShaders = []()
  {
    shaderMan.updateProjectionMatrices();
    shaderMan.updateViewMatrices();
  };
  engine.addToUpdate(updateShaders);

  engine.run();

}