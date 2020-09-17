/*
addSkybox
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
#include <string>
#include <vector>
#include <Loop.h>
#include <memory>
#include <Skybox.h>

/**
 *  * This function is based off having skyboxes in an asset path with the name of the skybox being the folder name and the files within containg
 *  *                                 up.png
 *  * skybox files: back.png left.png front.png right.png
 *  *                                 down.png
 */
void addCubeSkybox()
{
  {
    //std::string skybox_dir;
    //std::vector<std::string> skybox_file_names;
    //LoadableAssets::getSkyboxCubeMapFiles("../assets/drakeq/", skybox_dir, skybox_file_names);

    // skybox file name order
    const std::string path = "../assets/skyboxes/drakeq/";
    const std::string order[6] = { "right", "left", "up", "down", "front", "back" };
    const std::string ext = ".png";
    std::vector<std::string> cubemapfiles;
    for (int j = 0; j < 6; ++j)
    {
      cubemapfiles.emplace_back(path + order[j] + ext);
    }

    const std::shared_ptr<AA::Skybox> skybox = std::make_shared<AA::Skybox>(cubemapfiles);
    LOOP->setSkybox(skybox);
  }
}