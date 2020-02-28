#pragma once
#include "LoadableAssets.h"
#include "../../world/AAGameObject.h"
#include "../../world/AAOGLGraphics.h"
#include "../../world/AASound.h"
#include <math.h>
#include <Random.h>

void addRandomWorld(AAWorld& worldEngine)
{
  static int i;
  static AAShaderManager shaderMan;
  static int triLightShader = shaderMan.addShader(
    "../shaders/vert_textured.glsl",
    "../shaders/frag_3coreLight.glsl"
  );
  static int noLightShader = shaderMan.addShader(
    "../shaders/vert_textured.glsl",
    "../shaders/frag_noLight.glsl"
  );

  // see what demo files we have
  std::string assetDir;
  std::vector<std::string> loadableModels;
  std::vector<std::string> loadableSounds;
  LoadableAssets loadableObjects;
  loadableObjects.getDemoConfig(assetDir, loadableModels, loadableSounds);
  static const int num3DObjects = loadableModels.size();
  static std::vector< AAGameObject > objs;
  if (num3DObjects != 0)
  {
    for (i = 0; i < num3DObjects; ++i)
    {
      objs.push_back(AAOGLGraphics::getInstance()->loadGameObjectWithAssimp(assetDir + loadableModels.at(i), true));
    }
  }

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
    shaderMan.updateProjectionMatrices();

    // random object placement
    if (num3DObjects != 0)
    {
      for (i = 0; i < num3DObjects; ++i)
      {
        // random rotation axis
        objs.at(i).changeRotateAxis(glm::vec3(
          mearly::roll1d3 == 1 ? 1 : 0, 
          mearly::roll1d3 == 2 ? 1 : 0,
          mearly::roll1d3 == 3 ? 1 : 0)
        );

        // random starting location
        objs.at(i).translate(glm::vec3(
          mearly::rolld20,
          mearly::rolld20,
          mearly::rolld20)
        );
      }
    }

    //gameObj2.translate(glm::vec3(0, 6, 0));
    //gameObj5.translate(glm::vec3(5,5,-5));
    //obj00.translate(glm::vec3(0, 4, 0));
    //obj01.translate(glm::vec3(-7, 0, 0));
    //gameObj8.translate(glm::vec3(0));
    //gameObj9.translate(glm::vec3(0));

    DirectionalLight dirLight;
    dirLight.Direction = glm::vec3(.15f, -1.f, .15f);
    dirLight.Ambient = glm::vec3(.6333f);
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
    spotLight.Ambient = glm::vec3(0.3f);
    spotLight.Diffuse = glm::vec3(.9f);
    spotLight.Specular = glm::vec3(1.f);
    spotLight.Constant = 1.0f;
    spotLight.Linear = .7f;
    spotLight.Quadratic = .009f;
    spotLight.CutOff = glm::cos(glm::radians(25.f));
    spotLight.OuterCutOff = glm::cos(glm::radians(38.f));
    setSpotLight(spotLight, shaderMan.getShader(triLightShader));

  };
  worldEngine.addToOnBegin(startFunc);

  const auto deltaMoveObjects = [](float dt)
  {
    if (num3DObjects != 0)
    {
      for (i = 0; i < num3DObjects; ++i)
      {
        //  rotate 
        objs.at(i).advanceRotation(glm::radians(dt * 10));

        // move towards player
        const glm::vec3 objToPlayerDir = glm::normalize(objs.at(i).getLocation() - AAViewport::getInstance()->getLocation());  //direction from object to player
        objs.at(i).advanceTranslate(dt * objToPlayerDir);

        // grow in size over time
        objs.at(i).advanceScale(glm::vec3(dt, dt, dt));
      }
    }
    //static float totalTime = 0;
    //totalTime += dt;
    //gameObj.translate(glm::vec3(dt * .5, 0, 0));
    //gameObj2.translate(glm::vec3(0, 0, -sin(totalTime)));
    //gameObj3.translate(glm::vec3(0, 0, 0));
    //gameObj3.rotate(dt * .5f, glm::vec3(0, 1, 0));
    //gameObj4.translate(glm::vec3(0, 0, sin(totalTime)));
    //gameObj5.rotate(dt * .3f, glm::vec3(0, 1, 0));
    //obj00.advanceRotation(glm::radians(dt * 5));
    //obj01.advanceRotation(glm::radians(dt * 10));

    //pointLight.Position = glm::vec3(0, 0, -sin(totalTime) * 10);
    //AAViewport::getInstance()->setPointLight(pointLight);

  };
  worldEngine.addToDeltaUpdate(deltaMoveObjects);

  const auto drawObjects = []()
  {
    if (sceneLighting) {
      for (i = 0; i < num3DObjects; ++i)
      {
        objs.at(i).draw(shaderMan.getShader(triLightShader));
      }
    }
    else
    {
      for (i = 0; i < num3DObjects; ++i)
      {
        objs.at(i).draw(shaderMan.getShader(noLightShader));
      }
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
      return true;
    }
    if (keys.l)
    {
      sceneLighting = false;
      return true;
    }
    return false;
  };
  worldEngine.addToTimedOutKeyHandling(lightingHotkeys);

}