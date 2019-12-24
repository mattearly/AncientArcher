#pragma once
#include <GLFW\glfw3.h>
#include "localModelPaths.h"
#include "dry/addSkybox.h"
#include "dry/addGodMovement.h"
#include "dry/addPreferedDefaults.h"
#include "../engine/AAEngine.h"
#include "../engine/AAGameObject.h"
#include "../engine/AAOGLGraphics.h"
#include "../engine/AAShaderManager.h"

void  testEngineFuncsDemo_1()
{

  AAEngine engine;

  addSkybox(engine, "drakeq");
  addGodMovement(engine);
  addPreferredDefaults(engine);

  static AAGameObject gameObj6 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model6, true);
  static AAGameObject gameObj9 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(test::model9, true);

  static AAShaderManager shaderMan;
  static int triLightShader = shaderMan.addShader("../AncientArcher/shader/vert_textured.glsl", "../AncientArcher/shader/frag_3coreLight.glsl");

  static PointLight pointLight;
  static SpotLight spotLight;

  auto startFunc = []()
  {
    AAViewport::getInstance()->setCurrentPosition(glm::vec3(0, 5, 10));
    AAViewport::getInstance()->setCurrentPitch(-20.f);
    AAViewport::getInstance()->setCurrentYaw(270.f);

    //gameObj2.translate(glm::vec3(0, 6, 0));
    //gameObj5.translate(glm::vec3(5,5,-5));
    gameObj6.translate(glm::vec3(0, 2, 0));
    //gameObj7.translate(glm::vec3(-51.625,0,0));
    //gameObj8.translate(glm::vec3(0));
    gameObj9.translate(glm::vec3(0));

    DirectionalLight dirLight;
    dirLight.Direction = glm::vec3(.15f, -1.f, .15f);
    dirLight.Ambient = glm::vec3(.03f);
    dirLight.Diffuse = glm::vec3(.2f);
    dirLight.Specular = glm::vec3(.5f);
    setDirectionalLight(dirLight, shaderMan.getShader(triLightShader));

    pointLight.Ambient = glm::vec3(0.9f);
    pointLight.Diffuse = glm::vec3(0.9f);
    pointLight.Specular = glm::vec3(1);
    pointLight.Constant = 1.f;
    pointLight.Linear = .09f;
    pointLight.Quadratic = .032f;
    pointLight.Position = glm::vec3(0, 0, 0);
    setPointLight(pointLight, shaderMan.getShader(triLightShader));

    spotLight.Position = glm::vec3(0, 5, 8.5);
    spotLight.Direction = glm::vec3(-1, 0, 0);
    spotLight.Ambient = glm::vec3(0);
    spotLight.Diffuse = glm::vec3(1.f);
    spotLight.Specular = glm::vec3(1.f);
    spotLight.Constant = 1.f;
    spotLight.Linear = .09f;
    spotLight.Quadratic = .015f;
    spotLight.CutOff = glm::cos(glm::radians(15.f));
    spotLight.OuterCutOff = glm::cos(glm::radians(19.f));
    setSpotLight(spotLight, shaderMan.getShader(triLightShader));

  };
  engine.addToOnBegin(startFunc);

  auto deltaMoveObjects = [](float dt)
  {
    static float totalTime = 0;
    totalTime += dt;
    //gameObj.translate(glm::vec3(dt * .5, 0, 0));
    //gameObj2.translate(glm::vec3(0, 0, -sin(totalTime)));
    //gameObj3.translate(glm::vec3(0, 0, 0));
    //gameObj3.rotate(dt * .5f, glm::vec3(0, 1, 0));
    //gameObj4.translate(glm::vec3(0, 0, sin(totalTime)));
    //gameObj5.rotate(dt * .3f, glm::vec3(0, 1, 0));
    gameObj6.rotate(dt * .6f, glm::vec3(0, 2.5, 0));

    //pointLight.Position = glm::vec3(0, 0, -sin(totalTime) * 10);
    //AAViewport::getInstance()->setPointLight(pointLight);

  };
  engine.addToDeltaUpdate(deltaMoveObjects);

  auto drawObjects = []()
  {
    //gameObj.draw();
    //gameObj2.draw();
    //gameObj3.draw();
    //gameObj4.draw();
    //gameObj5.draw();
    gameObj6.draw(shaderMan.getShader(triLightShader));
    //gameObj7.draw();
    //gameObj8.draw();
    gameObj9.draw(shaderMan.getShader(triLightShader));
  };
  engine.addToOnRender(drawObjects);

  auto updateShaders = []()
  {
    shaderMan.updateProjectionMatrices();
    shaderMan.updateViewMatrices();
  };
  engine.addToUpdate(updateShaders);

  switch (engine.run())
  {
  case -1:
    std::cout << "Keys not set.\n";
    break;
  case -2:
    std::cout << "Mouse not set.\n";
    break;
  case -3:
    std::cout << "Scroll not set.\n";
    break;
  default:
    break;
  }

}
