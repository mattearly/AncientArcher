#include "LoadableAssets.h"
#include <iostream>
#include <Windows.h>

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

        std::vector<std::string> all_files = get_all_files_names_within_folder(mModelFileDir);

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

        std::vector<std::string> all_files = get_all_files_names_within_folder(mSoundFileDir);

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
  // get path to demo files
  std::string thisFilePath = __FILE__;
#ifdef _WIN32
  thisFilePath.erase(thisFilePath.find_last_of("\\"));
  thisFilePath.erase(thisFilePath.find_last_of("\\"));
  thisFilePath.erase(thisFilePath.find_last_of("\\"));
  thisFilePath.erase(thisFilePath.find_last_of("\\") + 1);
  thisFilePath.append("assets\\");
#elif __linux__
//todo
#else
  //todo
#endif

  /////////////// gets full loadable demo model paths ///////////
  {
    std::string aType = "models";
    std::string assetFolderPath = thisFilePath;
    std::string assetExtension = ".obj";

    // todo: consider error checking
    mModels.clear();
    mModelFileDir.clear();

    mModelFileDir = assetFolderPath;
    std::vector<std::string> all_files = get_all_files_names_within_folder(mModelFileDir);

    // only save files of specified asset extension
    for (auto filename : all_files)
    {
      if (filename.find(assetExtension) != std::string::npos)
      {
        mModels.push_back(filename);
      }
    }

    {
      // debug: show all the models
      std::cout << "List of loadable demo models:\n\n";
      int count = 0;
      for (auto name : mModels)
      {
        std::cout << '\t' << count++ << ". " << name << '\n';
      }
      if (count == 0)
      {
        std::cout << "no demo models loadable.\n";
      }
    }
  }

  /////////////// gets full loadable demo sound paths ///////////
  {
    std::string aType = "sounds";
    std::string assetFolderPath = thisFilePath;
    std::string assetExtension = ".wav";

    // todo: consider error checking
    mSounds.clear();
    mSoundFileDir.clear();

    mSoundFileDir = assetFolderPath;
    std::vector<std::string> all_files = get_all_files_names_within_folder(mSoundFileDir);

    // only save files of specified asset extension
    for (auto filename : all_files)
    {
      if (filename.find(assetExtension) != std::string::npos)
      {
        mSounds.push_back(filename);
      }
    }


    // debug: show all the demo wav sounds
    std::cout << "List of loadable demo sounds:\n\n";
    int count = 0;
    for (auto name : mSounds)
    {
      std::cout << '\t' << count++ << ". " << name << '\n';
    }
    if (count == 0)
    {
      std::cout << "no demo .wav sounds loadable.\n";
    }

    // get loadable demo mp3 sounds
    assetExtension = ".mp3";
    // only save files of specified asset extension
    for (auto filename : all_files)
    {
      if (filename.find(assetExtension) != std::string::npos)
      {
        mSounds.push_back(filename);
      }
    }

    // debug: show all the demo mp3 sounds
    count = mSounds.size() - 1;
    for (auto name : mSounds)
    {
      std::cout << '\t' << count++ << ". " << name << '\n';
    }
    if (count == mSounds.size() -1)
    {
      std::cout << "no demo .mp3 sounds loadable.\n";
    }
  }
}

std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
  std::vector<std::string> names;
  std::string search_path = folder + "/*.*";
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