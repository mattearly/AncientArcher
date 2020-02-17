#include "LoadableAssets.h"
#include <iostream>
#include <Windows.h>
#include <filesystem>

std::string LoadableAssets::getModel(std::string objname) const
{
  if (mModels.empty())
    return "no model filenames :(";

  for (auto name : mModels)
  {
    if (name == objname)
    {
      return mModelFileDir + name;
    }
  }

  return "model file not found :(";
}

std::string LoadableAssets::getModel(int which) const
{
  if (which > mModels.size() - 1)
    return "out of range";

  return mModelFileDir + mModels.at(which);
}

std::vector<std::string> LoadableAssets::getAllModels() const
{
  std::vector<std::string> pathedModels;

  for (auto s : mModels)
  {
    pathedModels.push_back(mModelFileDir + s);
  }

  return pathedModels;
}

std::size_t LoadableAssets::getNumLoadableModels() const noexcept
{
  return mModels.size();
}

std::string LoadableAssets::getSound(std::string soundname) const
{
  if (mSounds.empty())
    return "no sound filenames :(";

  for (auto name : mSounds)
  {
    if (name == soundname)
    {
      return mSoundFileDir + name;
    }
  }

  return "sound file not found :(";
}

std::string LoadableAssets::getSound(int which) const
{
  if (which > mSounds.size() - 1)
    return "out of range";

  return mSoundFileDir + mSounds.at(which);
}

std::vector<std::string> LoadableAssets::getAllSounds() const
{
  std::vector<std::string> pathedSounds;

  for (auto s : mSounds)
  {
    pathedSounds.push_back(mSoundFileDir + s);
  }

  return pathedSounds;
}

std::size_t LoadableAssets::getNumLoadableSounds() const noexcept
{
  return mSounds.size();
}

/**
 * Reads in a config file with the follow characteristics:
 *   line one: type of assets (currently 'sounds' and 'models' are supported)
 *   line two: path to asset folder
 *   line three: extension type (ex.  .obj  .wav  )
 * After reading a config file model or sound list is updated with current available options.
 * @in pathtoconfig  path to the configuration file
 */
void LoadableAssets::loadConfig(std::string pathtoconfig)
{
  {
    // open file
    std::ifstream ins(pathtoconfig);
    if (ins.is_open())
    {
      std::string lineBuf;
      std::string aType;
      getline(ins, aType);   // LINE 1 -- asset type
      if (aType == "models")
      {
        mModels.clear();
        mModelFileDir.clear();
        getline(ins, mModelFileDir);   // LINE 2 -- path to storage folder

        std::vector<std::string> all_files = win_GetAllFileNamesInFolder(mModelFileDir);

        getline(ins, lineBuf);  // LINE 3 -- asset extension

        // only save files of specified asset extension
        for (auto filename : all_files)
        {
          if (filename.find(lineBuf) != std::string::npos)
          {
            mModels.push_back(filename);
          }
        }


        // debug: show all the models
        std::cout << "List of loadable models:\n\n";
        int count = 0;
        for (auto name : mModels)
        {
          std::cout << '\t' << count++ << ". " << name << '\n';
        }
      }
      else if (aType == "sounds")
      {
        mSounds.clear();
        mSoundFileDir.clear();
        getline(ins, mSoundFileDir);   // LINE 2 -- path to storage folder
        getline(ins, lineBuf);  // LINE 3 -- end file type

        std::vector<std::string> all_files = win_GetAllFileNamesInFolder(mSoundFileDir);

        getline(ins, lineBuf);  // LINE 3 -- asset extension

        // only save files of specified asset extension
        for (auto filename : all_files)
        {
          if (filename.find(lineBuf) != std::string::npos)
          {
            mSounds.push_back(filename);
          }
        }

        // debug: show all the sounds
        std::cout << "List of loadable sounds:\n\n";
        int count = 0;
        for (auto name : mSounds)
        {
          std::cout << '\t' << count++ << ". " << name << '\n';
        }
      }
    }
    else
    {
      std::cout << "failed to open your config file @ \n\t" << pathtoconfig << '\n';
    }
  }
}

/**
 *  Prepares the demo files in this project for loading.
 *  based on the assets folder at the root of this project: $(ProjectDir)assets
 */
void LoadableAssets::loadDemoConfig()
{
  std::filesystem::path demoFilePath = "../assets/";  // asset path
  if (std::filesystem::exists(demoFilePath))
  {
    mModelFileDir = mSoundFileDir = demoFilePath.string();
  }
  else
  {
    std::cout << "ERROR::DEMO file path does not exist\n";
    return;
  }

  std::vector<std::filesystem::path> all_files = cpp17_GetAllFileNamesInFolder(demoFilePath);  // all the files in the path

  if (all_files.size() == 0)
  {
    std::cout << "no assets found, ending search for demo assets\n";
    return;
  }
  else  // populate legal assets paths
  {
    //debug - list all files
    for (const auto& file : all_files)
    {
      std::cout << "all files: " << file << '\n';
    }

    // a scope to populate mModels with the .obj paths in demoFilePath
    {
      std::string assetExtension = ".obj";
      mModels.clear();
      // = demoFilePath.string().substr(0, demoFilePath.string().find_last_of("/") + 1);

      // only save files of specified asset extension
      for (const auto& filepath : all_files)
      {
        std::string filename = filepath.string().substr(10);
        if (filename.find(assetExtension) != std::string::npos)
        {
          mModels.push_back(filename);
        }
      }

      //debug - list found model files
      std::cout << "number of models: " << mModels.size() << '\n';
      for (auto file : mModels)
      {
        std::cout << assetExtension << " files: " << file << '\n';
      }
    }

    // a scope to populate mSounds with the .wav and .mp3 paths in demoFilePath
    {
      std::string assetExtension = ".wav";
      mSounds.clear();
      for (const auto& filepath : all_files)
      {
        std::string filename = filepath.string().substr(10, 100);
        if (filename.find(assetExtension) != std::string::npos)
        {
          mSounds.push_back(filename);
        }
      }
      // get loadable demo mp3 sounds
      assetExtension = ".mp3";
      // only save files of specified asset extension
      for (const auto& filepath : all_files)
      {
        std::string filename = filepath.string().substr(10, 100);
        if (filename.find(assetExtension) != std::string::npos)
        {
          mSounds.push_back(filename);
        }
      }
      //debug - list found sounds files
      std::cout << "number of sounds: " << mSounds.size() << '\n';
      for (auto file : mSounds)
      {
        std::cout << "sound files: " << file << '\n';
      }
    }
  }
}

#ifdef WIN32
std::vector<std::string> win_GetAllFileNamesInFolder(std::string path)
{
  std::vector<std::string> names;
  std::string search_path = path + "/*.*";
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        names.push_back(fd.cFileName);
      }
    } while (::FindNextFile(hFind, &fd));
    ::FindClose(hFind);
  }
  return names;
}
#endif

/**
 * Access all the file names in a folder (non-recursively) and return a vector full of all the paths.
 * Returns an empty vector if the path doesn't exist or if there are no items in the directory.
 */
std::vector<std::filesystem::path> cpp17_GetAllFileNamesInFolder(std::filesystem::path path)
{
  std::vector<std::filesystem::path> fileNamePaths;

  if (!exists(path))
  {
    return fileNamePaths;  // no items within just a empty vector
  }

  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    fileNamePaths.push_back(entry);
  }

  return fileNamePaths;
}
