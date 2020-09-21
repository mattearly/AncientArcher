/*
LoadableAssets
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
#include "LoadableAssets.h"
#include "Filesystem.h"
#include <iostream>

/**
 *  Prepares the demo files in this project for loading.
 *  based on the assets folder at the root of this project: $(ProjectDir)assets
 */
void LoadableAssets::getDemoConfig(std::string& out_dir, std::vector<std::string>& out_model_list, std::vector<std::string>& out_sound_list, const std::string assetPath)
{
	std::filesystem::path demoFilePath = assetPath;
	const std::size_t PATHSIZE = demoFilePath.string().size();
	if (std::filesystem::exists(demoFilePath))
	{
		out_dir = demoFilePath.string();
	}
	else
	{
		return;  //todo: log error
	}

	// all the files in the path
	std::vector<std::filesystem::path> all_files = cpp17_GetAllFileNamesInFolder(demoFilePath);

	if (all_files.size() == 0)
	{
		return;  //todo: log error
	}
	else  // populate legal assets paths
	{
		// scope to populate mModels
		{
			std::string assetExtension = ".obj";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_model_list.push_back(filename);
				}
			}

			assetExtension = ".dae";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_model_list.push_back(filename);
				}
			}

			assetExtension = ".fbx";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_model_list.push_back(filename);
				}
			}

			assetExtension = ".gltf";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_model_list.push_back(filename);
				}
			}

			assetExtension = ".glb";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_model_list.push_back(filename);
				}
			}
		}

		// scope to populate mSounds with the .wav and .mp3 paths in demoFilePath
		{
			std::string assetExtension = ".wav";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_sound_list.push_back(filename);
				}
			}
			assetExtension = ".mp3";
			for (const auto& filepath : all_files)
			{
				std::string filename = filepath.string().substr(PATHSIZE);
				if (filename.find(assetExtension) != std::string::npos)
				{
					out_sound_list.push_back(filename);
				}
			}
		}
	}
}

///**
// *  Populates return_dir and return_file_list with the skybox file stuff.
// */
//void LoadableAssets::getSkyboxCubeMapFiles(const std::string& containingFolderPath, std::string& return_dir, std::vector<std::string>& return_file_list)
//{
//  std::filesystem::path skyboxFilePath = containingFolderPath;  // asset path
//  const std::size_t PATHSIZE = skyboxFilePath.string().size();
//  if (std::filesystem::exists(skyboxFilePath))
//  {
//    return_dir = skyboxFilePath.string();
//  }
//  else
//  {
//    return;  //todo: log error
//  }
//
//  std::vector<std::filesystem::path> all_files = cpp17_GetAllFileNamesInFolder(skyboxFilePath);  // all the files in the path
//
//  if (all_files.size() != 6)
//  {
//    return;  //todo: log error
//  }
//  else  // populate legal assets paths
//  {
//    // scope to populate mModels with the .obj paths in demoFilePath
//    {
//      std::string assetExtension = ".png";
//
//      // only save files of specified asset extension
//      for (const auto& filepath : all_files)
//      {
//        std::string filename = filepath.string().substr(PATHSIZE);
//        if (filename.find(assetExtension) != std::string::npos)
//        {
//          return_file_list.push_back(filename);
//        }
//      }
//    }
//  }
//}

//#ifdef WIN32
//std::vector<std::string> win_GetAllFileNamesInFolder(std::string path)
//{
//  std::vector<std::string> names;
//  std::string search_path = path + "/*.*";
//  WIN32_FIND_DATA fd;
//  HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
//  if (hFind != INVALID_HANDLE_VALUE) {
//    do {
//      // read all (real) files in current folder
//      // , delete '!' read other 2 default folder . and ..
//      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//        names.push_back(fd.cFileName);
//      }
//    } while (::FindNextFile(hFind, &fd));
//    ::FindClose(hFind);
//  }
//  return names;
//}
//#endif