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

  LoadableAssets assets;
  assets.loadDemoConfig();
  static AAGameObject gameObj6 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(assets.getModel("6ColorSquare.obj"), true);
  gameObj6.changeRotateAxis(glm::vec3(0, 1, 0));
  //static AAGameObject gameObj9 = AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(objs.getModel(6), true);

  static AASound sound;
  sound.addSoundEffects(assets.getAllSounds());

  worldEngine.setKeyTimeoutLength(.1f);

  auto soundHotkeys = [](AAKeyBoardInput& keys)
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

  auto startFunc = []()
  {
    AAViewport::getInstance()->setCurrentPosition(glm::vec3(0, 5, 10));
    AAViewport::getInstance()->setCurrentPitch(-20.f);
    AAViewport::getInstance()->setCurrentYaw(270.f);

    shaderMan.updateProjectionMatrices();

    //gameObj2.translate(glm::vec3(0, 6, 0));
    //gameObj5.translate(glm::vec3(5,5,-5));
    gameObj6.translate(glm::vec3(0, 4, 0));
    //gameObj7.translate(glm::vec3(-51.625,0,0));
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
    gameObj6.advanceRotation(glm::radians(dt * 5));

    //pointLight.Position = glm::vec3(0, 0, -sin(totalTime) * 10);
    //AAViewport::getInstance()->setPointLight(pointLight);

  };
  worldEngine.addToDeltaUpdate(deltaMoveObjects);

  auto drawObjects = []()
  {
    if (sceneLighting) {
      gameObj6.draw(shaderMan.getShader(triLightShader));
      //gameObj9.draw(shaderMan.getShader(triLightShader));
    }
    else
    {
      gameObj6.draw(shaderMan.getShader(noLightShader));
      //gameObj9.draw(shaderMan.getShader(noLightShader));
    }
  };
  worldEngine.addToOnRender(drawObjects);

  auto updateShaders = []()
  {
    //shaderMan.updateProjectionMatrices();  //done once and on viewport resize

    //following 4 lines need done on movement
    shaderMan.updateViewMatrices();
    spotLight.Position = *VIEWPORT->getPosition();
    spotLight.Direction = *VIEWPORT->getFront();
    setSpotLight(spotLight, shaderMan.getShader(triLightShader));
  };
  worldEngine.addToUpdate(updateShaders);

  auto lightingHotkeys = [](AAKeyBoardInput& keys)
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