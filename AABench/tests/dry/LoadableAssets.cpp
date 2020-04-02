#include "LoadableAssets.h"
#include "Filesystem.h"
#include <iostream>

/**
 *  Prepares the demo files in this project for loading.
 *  based on the assets folder at the root of this project: $(ProjectDir)assets
 */
void LoadableAssets::getDemoConfig(std::string& out_dir, std::vector<std::string>& out_model_list, std::vector<std::string>& out_sound_list)
{
  std::filesystem::path demoFilePath = "../assets/";  // asset path
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
  std::vector<std::filesystem::path> all_files = mearly::FileSystem::cpp17_GetAllFileNamesInFolder(demoFilePath);

  if (all_files.size() == 0)
  {
    return;  //todo: log error
  }
  else  // populate legal assets paths
  {
    // scope to populate mModels
    {
      std::string assetExtension = ".obj";
      //for (const auto& filepath : all_files)
      //{
      //  std::string filename = filepath.string().substr(PATHSIZE);
      //  if (filename.find(assetExtension) != std::string::npos)
      //  {
      //    out_model_list.push_back(filename);
      //  }
      //}

      //assetExtension = ".dae";
      //for (const auto& filepath : all_files)
      //{
      //  std::string filename = filepath.string().substr(PATHSIZE);
      //  if (filename.find(assetExtension) != std::string::npos)
      //  {
      //    out_model_list.push_back(filename);
      //  }
      //}

      assetExtension = ".fbx";
      for (const auto& filepath : all_files)
      {
        std::string filename = filepath.string().substr(PATHSIZE);
        if (filename.find(assetExtension) != std::string::npos)
        {
          out_model_list.push_back(filename);
        }
      }

      //assetExtension = ".gltf";
      //for (const auto& filepath : all_files)
      //{
      //  std::string filename = filepath.string().substr(PATHSIZE);
      //  if (filename.find(assetExtension) != std::string::npos)
      //  {
      //    out_model_list.push_back(filename);
      //  }
      //}  
      
      //assetExtension = ".glb";
      //for (const auto& filepath : all_files)
      //{
      //  std::string filename = filepath.string().substr(PATHSIZE);
      //  if (filename.find(assetExtension) != std::string::npos)
      //  {
      //    out_model_list.push_back(filename);
      //  }
      //}


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
//static std::vector<std::string> win_GetAllFileNamesInFolder(std::string path)
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

