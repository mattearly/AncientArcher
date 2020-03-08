#pragma once
#include "LoadableAssets.h"
#include <math.h>
#include "Random.h"
#include "../../AAEngine.h"
#include "../../core/soundPlugin/AASound.h"
#include "../../core/objectSys/shaderSys/AALights.h"

extern AAWorld mainWorld;

void addRandomWorld(int cam_id)
{
  static int camID = cam_id;

  // shader with pure diffuse on textures
  static int mainShaderId = mainWorld.addShader("../shaders/vert_combinedLight.glsl", "../shaders/frag_combinedLight.glsl");
  //static int mainShaderId = mainWorld.addShader("../shaders/vert_default.glsl", "../shaders/frag_noLight.glsl");

  static DirectionalLight directional_light;
  static PointLight point_lights[MAXPOINTLIGHTS];
  static SpotLight spot_lights[MAXSPOTLIGHTS];

  directional_light.Direction = glm::vec3(.15f, -1.f, .15f);
  directional_light.Ambient = glm::vec3(.6333f);
  directional_light.Diffuse = glm::vec3(.05f);
  directional_light.Specular = glm::vec3(.5f);
  setDirectionalLight(directional_light, mainWorld.getShader(mainShaderId));

  for (int i = 0; i < MAXPOINTLIGHTS; i++)
  {
    point_lights[i].Ambient = glm::vec3(0.9f);
    point_lights[i].Diffuse = glm::vec3(0.9f);
    point_lights[i].Specular = glm::vec3(1);
    point_lights[i].Constant = 1.f;
    point_lights[i].Linear = .09f;
    point_lights[i].Quadratic = .032f;
    point_lights[i].Position = glm::vec3(0+i, 0, 0+i);
    numPointLightsInUse++;
    std::cout << "point light: " << numPointLightsInUse << '\n';
    setPointLight(point_lights[i], i, mainWorld.getShader(mainShaderId));
  }

  for (int i = 0; i < MAXSPOTLIGHTS; i++)
  {
    spot_lights[i].Position = glm::vec3(0+i, 5, 8.5);
    spot_lights[i].Direction = glm::vec3(-1, 0, 0);
    spot_lights[i].Ambient = glm::vec3(0.3f);
    spot_lights[i].Diffuse = glm::vec3(.9f);
    spot_lights[i].Specular = glm::vec3(1.f);
    spot_lights[i].Constant = 1.0f;
    spot_lights[i].Linear = .7f;
    spot_lights[i].Quadratic = .009f;
    spot_lights[i].CutOff = glm::cos(glm::radians(25.f));
    spot_lights[i].OuterCutOff = glm::cos(glm::radians(38.f));
    numSpotLightsInUse++;
    std::cout << "spot light: " << numSpotLightsInUse << '\n';
    setSpotLight(spot_lights[i], mainWorld.getShader(mainShaderId));
  }

//const auto updateShaders = []()
//{
//  spotLight.Position = *mainWorld.getCamera(camID).getPosition();
//  spotLight.Direction = *mainWorld.getCamera(camID).getFront();
//  setSpotLight(spotLight, mainWorld.getShader(mainShaderId));
//};
//mainWorld.addToUpdate(updateShaders);

  // see what demo files we have in our data dir
  std::string assetDir;
  std::vector<std::string> loadableModels;
  std::vector<std::string> loadableSounds;

  LoadableAssets pathsToLoadables;
  pathsToLoadables.getDemoConfig(assetDir, loadableModels, loadableSounds);
  static const int numLoadableModels = loadableModels.size();  // produces warning

  if (numLoadableModels > 0)
  {
    // vars to use
    constexpr float floorHeight = -10.f; 
    constexpr float roomWidth = 40.f;
    constexpr float ceilingHeight = 40.f;

    // for each loadable model detected
    for (int i = 0; i < numLoadableModels; ++i)
    {
      // ------------ multi instance example ---------------------------------
      // add multiple objects by giving a vec of instance details
      constexpr int num_instances = 100;
      // make some details for the object instances
      std::vector<InstanceDetails> details;
      for (int j = 0; j < num_instances; ++j)
      {
        details.push_back(InstanceDetails());
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
}