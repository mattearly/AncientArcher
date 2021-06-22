#pragma once
#include <string>
#include <vector>
#include <memory>
#include <AncientArcher/AncientArcher.h>
void setskybox()
{
  const std::string skyboxfolder = "..\\ExampleProject\\res\\skybox\\";
  const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
  const std::string skyboxfileext = ".jpg";
  std::vector<std::string> cubemapfiles;
  cubemapfiles.reserve(6);
  for (int j = 0; j < 6; ++j)
    cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
  AA::SetSkybox(cubemapfiles);

}