#pragma once
#include <string>
#include <cassert>
#include <vector>
#include <memory>
#include "../engine/AASkybox.h"
#include "../engine/AAEngine.h"


void addskybox(AAEngine& engine, const std::string& n)
{
  int skybox_choice = 0;
  const int numOptions = 2;
  std::string name[numOptions] = { "nordic", "drakeq" };
  if (n == "nordic")
  {
    skybox_choice = 0;
  }
  else if (n == "drakeq")
  {
    skybox_choice = 1;
  }
  assert(skybox_choice < numOptions);
  {
    // this is all based on where I am storing the data for cubemaps for testing
    std::string folderpath = "C:/Users/matt/Dropbox_me298414/Dropbox/SkyboxCubemaps/";
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