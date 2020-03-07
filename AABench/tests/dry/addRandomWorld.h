#pragma once
#include "LoadableAssets.h"
#include <math.h>
#include "Random.h"
#include "../../AAEngine.h"
#include "../../core/soundPlugin/AASound.h"
#include "../../core/objectSys/shaderSys/AALights.h"

static int i = 0;

extern AAWorld mainWorld;

void addRandomWorld(int cam_id)
{
  static int camID = cam_id;
  static int mainShaderId = mainWorld.addShader("../shaders/vert_default.glsl", "../shaders/frag_noLight.glsl");  // shader with pure diffuse on textures

  // see what demo files we have in our data dir
  std::string assetDir;
  std::vector<std::string> loadableModels;
  std::vector<std::string> loadableSounds;

  LoadableAssets pathsToLoadables;
  pathsToLoadables.getDemoConfig(assetDir, loadableModels, loadableSounds);
  static const int numLoadableModels = loadableModels.size();
  log(numLoadableModels);

  if (numLoadableModels > 0)
  {
    // vars to use
    constexpr int floorHeight = -10.f;
    constexpr int roomWidth = 40.f;
    constexpr int ceilingHeight = 40.f;

    // for each loadable model detected
    for (i = 0; i < numLoadableModels; ++i)
    {
      // ------------ multi instance example ---------------------------------
      // add multiple obects by giving a vec of instance details
      constexpr int num_instances = 100;
      // make some details for the object instances
      std::vector<InstanceDetails> details;
      for (int j = 0; j < num_instances; ++j)
      {
        const InstanceDetails inst;
        //inst.Translate = glm::vec3(mearly::Random::NTKR(-roomWidth, roomWidth), floorHeight, mearly::Random::NTKR(-roomWidth, roomWidth));
        details.push_back(inst);
      }

      // add the object along with a detail vector
      mainWorld.addObject(std::string(assetDir + loadableModels.at(i)).c_str(), camID, mainShaderId, details);

      // translate the objects around randomly
      for (int j = 0; j < num_instances; ++j) {
        mainWorld.getGameObject(i).translateTo(
          glm::vec3(mearly::Random::NTKR(-roomWidth, roomWidth), floorHeight, mearly::Random::NTKR(-roomWidth, roomWidth)),
          j
        );
      }


      // ---------------------- single instance exmaple -------------------
      //// add an object
      //mainWorld.addObject(std::string(assetDir + loadableModels.at(i)).c_str(), camID, mainShaderId);
      //// place in a random x/z location within room width
      //mainWorld.getGameObject(i).translateTo(
      //  glm::vec3(
      //    mearly::Random::NTKR(-roomWidth, roomWidth),
      //    floorHeight,
      //    mearly::Random::NTKR(-roomWidth, roomWidth)
      //  )
      //);
    }
  }

  static AASound sound;
  sound.addSoundEffects(assetDir, loadableSounds);

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

  //static PointLight pointLight; 
  //static SpotLight spotLight;

  //DirectionalLight dirLight;
  //dirLight.Direction = glm::vec3(.15f, -1.f, .15f);
  //dirLight.Ambient = glm::vec3(.6333f);
  //dirLight.Diffuse = glm::vec3(.05f);
  //dirLight.Specular = glm::vec3(.5f);
  //setDirectionalLight(dirLight, mainWorld.getShader(mainShaderId));

  //pointLight.Ambient = glm::vec3(0.9f);
  //pointLight.Diffuse = glm::vec3(0.9f);
  //pointLight.Specular = glm::vec3(1);
  //pointLight.Constant = 1.f;
  //pointLight.Linear = .09f;
  //pointLight.Quadratic = .032f;
  //pointLight.Position = glm::vec3(0, 0, 0);
  //setPointLight(pointLight, mainWorld.getShader(mainShaderId));

  //spotLight.Position = glm::vec3(0, 5, 8.5);
  //spotLight.Direction = glm::vec3(-1, 0, 0);
  //spotLight.Ambient = glm::vec3(0.3f);
  //spotLight.Diffuse = glm::vec3(.9f);
  //spotLight.Specular = glm::vec3(1.f);
  //spotLight.Constant = 1.0f;
  //spotLight.Linear = .7f;
  //spotLight.Quadratic = .009f;
  //spotLight.CutOff = glm::cos(glm::radians(25.f));
  //spotLight.OuterCutOff = glm::cos(glm::radians(38.f));
  //setSpotLight(spotLight, mainWorld.getShader(mainShaderId));

  //const auto updateShaders = []()
  //{
  //  spotLight.Position = *mainWorld.getCamera(camID).getPosition();
  //  spotLight.Direction = *mainWorld.getCamera(camID).getFront();
  //  setSpotLight(spotLight, mainWorld.getShader(mainShaderId));
  //};
  //mainWorld.addToUpdate(updateShaders);

}