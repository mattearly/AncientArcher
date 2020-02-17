#pragma once
#include "LoadableAssets.h"
#include "../../world/AAGameObject.h"
#include "../../world/AAOGLGraphics.h"
#include "../../world/AASound.h"

void addRandomWorld(AAWorld& worldEngine)
{
  static AAShaderManager shaderMan;
  static int triLightShader = shaderMan.addShader(
    "../shaders/vert_textured.glsl",
    "../shaders/frag_3coreLight.glsl"
  );
  static int noLightShader = shaderMan.addShader(
    "../shaders/vert_textured.glsl",
    "../shaders/frag_noLight.glsl"
  );
  
  std::string assetDir;
  std::vector<std::string> loadableModels;
  std::vector<std::string> loadableSounds;

  LoadableAssets::loadDemoConfig(assetDir, loadableModels, loadableSounds);
  static AAGameObject obj00 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(assetDir + loadableModels.at(0), true);
  obj00.changeRotateAxis(glm::vec3(0, 1, 0));
  static AAGameObject obj01 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(assetDir + loadableModels.at(1), true);
  obj01.changeRotateAxis(glm::vec3(0, 0, 1));

  static AASound sound;
  sound.addSoundEffects(assetDir, loadableSounds);

  worldEngine.setKeyTimeoutLength(.3f);

  const auto soundHotkeys = [](AAKeyBoardInput& keys)
  {
    if (keys.mouseButton1)
    {
      sound.playSoundEffect(0);
      return true;
    }
    return false;
  };
  worldEngine.addToTimedOutKeyHandling(soundHotkeys);

  static bool sceneLighting = true;

  static PointLight pointLight;
  static SpotLight spotLight;

  const auto startFunc = []()
  {
    AAViewport::getInstance()->setCurrentPosition(glm::vec3(0, 10, 10));
    AAViewport::getInstance()->setCurrentPitch(-20.f);
    AAViewport::getInstance()->setCurrentYaw(270.f);

    shaderMan.updateProjectionMatrices();

    //gameObj2.translate(glm::vec3(0, 6, 0));
    //gameObj5.translate(glm::vec3(5,5,-5));
    obj00.translate(glm::vec3(0, 4, 0));
    obj01.translate(glm::vec3(-7,0,0));
    //gameObj8.translate(glm::vec3(0));
    //gameObj9.translate(glm::vec3(0));

    DirectionalLight dirLight;
    dirLight.Direction = glm::vec3(.15f, -1.f, .15f);
    dirLight.Ambient = glm::vec3(.03f);
    dirLight.Diffuse = glm::vec3(.05f);
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
    spotLight.Ambient = glm::vec3(0.f);
    spotLight.Diffuse = glm::vec3(1.f);
    spotLight.Specular = glm::vec3(1.f);
    spotLight.Constant = 1.f;
    spotLight.Linear = .09f;
    spotLight.Quadratic = .009f;
    spotLight.CutOff = glm::cos(glm::radians(28.f));
    spotLight.OuterCutOff = glm::cos(glm::radians(35.f));
    setSpotLight(spotLight, shaderMan.getShader(triLightShader));

  };
  worldEngine.addToOnBegin(startFunc);

  const auto deltaMoveObjects = [](float dt)
  {
    static float totalTime = 0;
    totalTime += dt;

    //gameObj.translate(glm::vec3(dt * .5, 0, 0));
    //gameObj2.translate(glm::vec3(0, 0, -sin(totalTime)));
    //gameObj3.translate(glm::vec3(0, 0, 0));
    //gameObj3.rotate(dt * .5f, glm::vec3(0, 1, 0));
    //gameObj4.translate(glm::vec3(0, 0, sin(totalTime)));
    //gameObj5.rotate(dt * .3f, glm::vec3(0, 1, 0));
    obj00.advanceRotation(glm::radians(dt * 5));
    obj01.advanceRotation(glm::radians(dt * 10));

    //pointLight.Position = glm::vec3(0, 0, -sin(totalTime) * 10);
    //AAViewport::getInstance()->setPointLight(pointLight);

  };
  worldEngine.addToDeltaUpdate(deltaMoveObjects);

  const auto drawObjects = []()
  {
    if (sceneLighting) {
      obj00.draw(shaderMan.getShader(triLightShader));
      obj01.draw(shaderMan.getShader(triLightShader));
      //gameObj9.draw(shaderMan.getShader(triLightShader));
    }
    else
    {
      obj00.draw(shaderMan.getShader(noLightShader));
      obj01.draw(shaderMan.getShader(noLightShader));
      //gameObj9.draw(shaderMan.getShader(noLightShader));
    }
  };
  worldEngine.addToOnRender(drawObjects);

  const auto updateShaders = []()
  {
    //shaderMan.updateProjectionMatrices();  //done once and on viewport resize

    //following 4 lines need done on movement
    shaderMan.updateViewMatrices();
    spotLight.Position = *VIEWPORT->getPosition();
    spotLight.Direction = *VIEWPORT->getFront();
    setSpotLight(spotLight, shaderMan.getShader(triLightShader));
  };
  worldEngine.addToUpdate(updateShaders);

  const auto lightingHotkeys = [](AAKeyBoardInput& keys)
  {
    if (keys.k)
    {
      sceneLighting = true;
    }
    if (keys.l)
    {
      sceneLighting = false;
    }
  };
  worldEngine.addToKeyHandling(lightingHotkeys);

}