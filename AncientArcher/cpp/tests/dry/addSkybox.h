#pragma once
#include <string>
#include <cassert>
#include <vector>
#include <memory>

#include "../../engine/AAEngine.h"

void addSkybox(AAEngine& engine, const std::string& n)
{
  int skybox_choice = 0;
  const int numOptions = 3;
  std::string name[numOptions] = { "nordic", "drakeq", "normandy" };

  // THE OPTIONS IN MY ASSEST - YOU WILL NEED YOUR OWN SKYBOX FILES OR ASK
  if (n == "nordic")
  {
    skybox_choice = 0;
  }
  else if (n == "drakeq")
  {
    skybox_choice = 1;
  }
  else if (n == "normandy")
  {
    skybox_choice = 2;
  }
  assert(skybox_choice < numOptions);
  {
    // this is all based on where I am storing the data for cubemaps for testing
    std::string folderpath = "C:/Users/matt/Dropbox_me298414/Dropbox/SkyboxCubemaps/";  // my skybox asset path
                                                                                        

    //                                  UP
    // my skybox file order: BACK LEFT FRONT RIGHT
    //                                  DOWN
    // my skybox file names
    std::string order[6] = { "/right", "/left", "/up", "/down", "/front", "/back" };    
                                                                                        
    std::string extension = ".png";
    std::vector<std::string> cubemapfiles;
    for (int j = 0; j < 6; ++j)
    {
      cubemapfiles.emplace_back(folderpath + name[skybox_choice] + order[j] + extension);
    }
    std::shared_ptr<AASkybox> skybox = std::make_shared<AASkybox>(cubemapfiles);
    engine.setSkybox(skybox);
  }
}