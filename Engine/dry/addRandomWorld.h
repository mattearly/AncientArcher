/*
add Random World
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#pragma once
#include "LoadableAssets.h"
#include <math.h>

void addRandomWorld(AA::Loop& loop, int cam_id)
{
  static int camID = cam_id;
 
  loop.setRenderDistance(camID, 10000.f);

  static int mainShaderId = loop.addShader("../shaders/combinedLight.vert", "../shaders/combinedLight.frag");

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
  static const int numLoadableModels = (int)loadableModels.size();  // produces warning

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
      std::vector<AA::InstanceDetails> details;
      for (int j = 0; j < num_instances; ++j)
      {
        details.push_back(AA::InstanceDetails());
      }

      // add the object along with a detail vector
      loop.addObject(std::string(assetDir + loadableModels.at(i)).c_str(), camID, mainShaderId, details);

      // translate the objects around randomly
      for (int j = 0; j < num_instances; ++j) {
        loop.getGameObject(i).translateTo(
          glm::vec3(
            AA::Random::NTKR(-roomWidth, roomWidth), 
            floorHeight, 
            AA::Random::NTKR(-roomWidth, roomWidth)),
          j
        );
      }

      // ---------------------- single instance example -------------------
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

  static AA::Sound sound;
  sound.addSoundEffects(assetDir, loadableSounds);

  const auto soundHotkeys = [](AA::KeyboardInput& keys)
  {
    if (keys.mouseButton1)
    {
      sound.playSoundEffect(0);
      return true;
    }
    return false;
  };
  loop.addToTimedOutKeyHandling(soundHotkeys);


  static AA::DirectionalLight directional_light;
  static AA::PointLight point_lights[AA::MAXPOINTLIGHTS];
  static AA::SpotLight spot_lights[AA::MAXSPOTLIGHTS];
  const auto begin = []()
  { 
    std::cout << "onBegin\n";
    directional_light.Direction = glm::vec4(-.15f, -1.f, -.15f, 0.f);
    directional_light.Ambient = glm::vec4(.35f);
    directional_light.Diffuse = glm::vec4(.35f);
    directional_light.Specular = glm::vec4(1.0f);
    setDirectionalLight(directional_light, loop.getShader(mainShaderId));

    for (int i = 0; i < AA::MAXPOINTLIGHTS; i++)
    {
      point_lights[i].Ambient = glm::vec4(0.9f);
      point_lights[i].Diffuse = glm::vec4(0.9f);
      point_lights[i].Specular = glm::vec4(1);
      point_lights[i].Constant = 1.f;
      point_lights[i].Linear = .09f;
      point_lights[i].Quadratic = .032f;
      point_lights[i].Position = glm::vec4(0 + i, 0, 0 + i, 0);
      AA::NUM_POINT_LIGHTS++;
      std::cout << "point light: " << AA::NUM_POINT_LIGHTS << '\n';
      setPointLight(point_lights[i], i, loop.getShader(mainShaderId));
    }

    for (int i = 0; i < AA::MAXSPOTLIGHTS; i++)
    {
      spot_lights[i].Position = glm::vec4(0 + i, 5, 8.5, 0);
      spot_lights[i].Direction = glm::vec4(-1, 0, 0, 0);
      spot_lights[i].Ambient = glm::vec4(0.3f);
      spot_lights[i].Diffuse = glm::vec4(.9f);
      spot_lights[i].Specular = glm::vec4(1.f);
      spot_lights[i].Constant = 1.0f;
      spot_lights[i].Linear = .7f;
      spot_lights[i].Quadratic = .009f;
      spot_lights[i].CutOff = glm::cos(glm::radians(25.f));
      spot_lights[i].OuterCutOff = glm::cos(glm::radians(38.f));
      AA::NUM_SPOT_LIGHTS++;
      std::cout << "spot light: " << AA::NUM_SPOT_LIGHTS << '\n';
      setSpotLight(spot_lights[i], i, loop.getShader(mainShaderId));
    }
  };
  loop.addToOnBegin(begin);
}