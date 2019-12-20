#pragma once
#include "../engine/AAEngine.h"
#include "modelPaths.h"
#include "../engine/AAGameObject.h"
#include "addskybox.h"
#include "addgodmovement.h"
#include "../engine/AAShaderManager.h"

void testWeirdLevel()
{
  AAEngine engine;

  addskybox(engine, "nordic");

  addgodmovement(engine);

  // add shaders to use
  static AAShaderManager shaderMan;
  static int noLightShader = shaderMan.addShader("../AncientArcher/shader/vert_textured.glsl", "../AncientArcher/shader/frag_noLight.glsl");

  // add object to draw
  static AAGameObject mountainGround = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model10, true);
  auto drawObjects = []()
  {
    mountainGround.draw(shaderMan.getShader(noLightShader));
  };
  engine.addToOnRender(drawObjects);

  auto updateShaders = []()
  {
    shaderMan.updateProjectionMatrices();
    shaderMan.updateViewMatrices();
  };
  engine.addToUpdate(updateShaders);

  // run engine
  engine.run();

}