#pragma once
#include "../../world/AAWorld.h"

/** 
 *  * This function is based off having skyboxes in an asset path with the name of the skybox being the folder name and the files within containg 
 *  *                                 up.png
 *  * skybox files: back.png left.png front.png right.png
 *  *                                 down.png
 */
void addDemoSkybox(AAWorld& engine)
{
  {
    //std::string skybox_dir;
    //std::vector<std::string> skybox_file_names;
    //LoadableAssets::getSkyboxCubeMapFiles("../assets/drakeq/", skybox_dir, skybox_file_names);

    // skybox file name order
    const std::string path = "../assets/drakeq/";
    const std::string order[6] = { "right", "left", "up", "down", "front", "back" };
    const std::string ext = ".png";
    std::vector<std::string> cubemapfiles;
    for (int j = 0; j < 6; ++j)
    {
      cubemapfiles.emplace_back(path + order[j] + ext);
    }

    const std::shared_ptr<AASkybox> skybox = std::make_shared<AASkybox>(cubemapfiles);
    engine.setSkybox(skybox);
  }
}