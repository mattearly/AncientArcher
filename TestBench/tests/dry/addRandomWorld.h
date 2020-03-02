#pragma once
#include "LoadableAssets.h"
#include <math.h>
#include <Random.h>
#include "../../AAEngine.h"
#include "../../core/soundPlugin/AASound.h"
#include "../../core/objectSys/shaderSys/AALights.h"


extern AAWorld mainWorld;

void addRandomWorld(int cam_id)
{

  static int camID = cam_id;

  static int i;
  // prepare shaders
//  static AAShaderManager shaderMan;
//  static int triLightShader = shaderMan.addShader(
//    "../shaders/vert_textured.glsl",
//    "../shaders/frag_3coreLight.glsl"
//  );
//  static int noLightShader = shaderMan.addShader(
//    "../shaders/vert_textured.glsl",
//    "../shaders/frag_noLight.glsl"
//  );

  // a 3 core light shader [1 directional, 1 spot, 1 point]
  //std::shared_ptr<AAOGLShader> shader = std::make_shared<AAOGLShader>(
  //  "../shaders/vert_textured.glsl", "../shaders/frag_3coreLight.glsl"
  //);

  //prepare a camera
  //std::shared_ptr<AACamera> cam = std::make_shared<AACamera>();
  static int mainShaderId;
  mainShaderId = mainWorld.addShader("../shaders/vert_textured.glsl", "../shaders/frag_3coreLight.glsl");


  // see what demo files we have in our data dir
  std::string assetDir;
  std::vector<std::string> loadableModels;
  std::vector<std::string> loadableSounds;

  LoadableAssets pathsToLoadables;
  pathsToLoadables.getDemoConfig(assetDir, loadableModels, loadableSounds);
  static const int num3DObjects = loadableModels.size();


  if (num3DObjects != 0)
  {
    for (i = 0; i < num3DObjects; ++i)
    {
      mainWorld.addObject(std::string(assetDir + loadableModels.at(i)).c_str(), cam_id, mainShaderId);
    }
  }

  static AASound sound;
  sound.addSoundEffects(assetDir, loadableSounds);

  mainWorld.setKeyTimeoutLength(.3f);
  const auto soundHotkeys = [](AAKeyBoardInput& keys)
  {
    if (keys.mouseButton1)
    {
      sound.playSoundEffect(0);
      return true;
    }
    return false;
  };
  mainWorld.addToTimedOutKeyHandling(soundHotkeys);

  //static bool sceneLighting = true;

  static PointLight pointLight; 
  static SpotLight spotLight;

  DirectionalLight dirLight;
  dirLight.Direction = glm::vec3(.15f, -1.f, .15f);
  dirLight.Ambient = glm::vec3(.6333f);
  dirLight.Diffuse = glm::vec3(.05f);
  dirLight.Specular = glm::vec3(.5f);
  setDirectionalLight(dirLight, mainWorld.getShader(mainShaderId));

  pointLight.Ambient = glm::vec3(0.9f);
  pointLight.Diffuse = glm::vec3(0.9f);
  pointLight.Specular = glm::vec3(1);
  pointLight.Constant = 1.f;
  pointLight.Linear = .09f;
  pointLight.Quadratic = .032f;
  pointLight.Position = glm::vec3(0, 0, 0);
  setPointLight(pointLight, mainWorld.getShader(mainShaderId));

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
  setSpotLight(spotLight, mainWorld.getShader(mainShaderId));


  const auto startFunc = []()
  {
    //shaderMan.updateProjectionMatrices();  //draw call should update them

    // random object placement
    //if (num3DObjects != 0)
    //{
    //  for (i = 0; i < num3DObjects; ++i)
    //  {
    //    // random rotation axis
    //    objs.at(i).changeRotateAxis(glm::vec3(
    //      mearly::roll1d3 == 1 ? 1 : 0, 
    //      mearly::roll1d3 == 1 ? 1 : 0,
    //      mearly::roll1d3 == 1 ? 1 : 0)
    //    );

    //    // random starting location
    //    objs.at(i).translate(glm::vec3(
    //      mearly::rolld20,
    //      mearly::rolld20,
    //      mearly::rolld20)
    //    );
    //  }
    //}

    //gameObj2.translate(glm::vec3(0, 6, 0));
    //gameObj5.translate(glm::vec3(5,5,-5));
    //obj00.translate(glm::vec3(0, 4, 0));
    //obj01.translate(glm::vec3(-7, 0, 0));
    //gameObj8.translate(glm::vec3(0));
    //gameObj9.translate(glm::vec3(0));

  };
  mainWorld.addToOnBegin(startFunc);

  const auto deltaMoveObjects = [](float dt)
  {
    if (num3DObjects != 0)
    {
      for (i = 0; i < num3DObjects; ++i)
      {
        //  rotate 
        //objs.at(i).advanceRotation(glm::radians(dt * 10));

        // move towards player
        //const glm::vec3 objToPlayerDir = glm::normalize(objs.at(i).getLocation() - cam.getLocation());  //direction from object to player
        //objs.at(i).advanceTranslate(dt * objToPlayerDir);

        // grow in size over time
        //objs.at(i).advanceScale(glm::vec3(dt, dt, dt));
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
  mainWorld.addToDeltaUpdate(deltaMoveObjects);

  //const auto drawObjects = []()
  //{
  //  if (sceneLighting) {
  //    for (i = 0; i < num3DObjects; ++i)
  //    {
  //      objs.at(i).draw(shaderMan.getShader(triLightShader), mainCamera);
  //    }
  //  }
  //  else
  //  {
  //    for (i = 0; i < num3DObjects; ++i)
  //    {
  //      objs.at(i).draw(shaderMan.getShader(noLightShader), mainCamera);
  //    }
  //  }
  //};
  //mainWorld.addToOnRender(drawObjects);

  const auto updateShaders = []()
  {
    //shaderMan.updateProjectionMatrices();  //done once and on viewport resize

    //following 4 lines need done on movement to keep the spot light at the player view center
    //shaderMan.updateViewMatrices();
    spotLight.Position = *mainWorld.getCamera(camID).getPosition();
    spotLight.Direction = *mainWorld.getCamera(camID).getFront();
    setSpotLight(spotLight, mainWorld.getShader(mainShaderId));
  };
  mainWorld.addToUpdate(updateShaders);

  //const auto lightingHotkeys = [](AAKeyBoardInput& keys)
  //{
  //  if (keys.k)
  //  {
  //    sceneLighting = true;
  //    return true;
  //  }
  //  if (keys.l)
  //  {
  //    sceneLighting = false;
  //    return true;
  //  }
  //  return false;
  //};
  //mainWorld.addToTimedOutKeyHandling(lightingHotkeys);

}