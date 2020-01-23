#include "LoadableAssets.h"
#include <iostream>
#include <mearly/getFilenamesWithinFolder.h>

std::string LoadableAssets::getModel(std::string objname) const
{
  if (mModels.empty())
    return "no model filenames :(";

  for (auto name : mModels)
  {
    if (name == objname)
    {
      return mModelFiledir + name;
    }
  }

  return "model file not found :(";
}

std::string LoadableAssets::getModel(int which) const
{
  if (which > mModels.size())
    return "out of range";

  return mModelFiledir + mModels.at(which);
}

std::string LoadableAssets::getSound(std::string soundname) const
{
  if (mSounds.empty())
    return "no sound filenames :(";

  for (auto name : mSounds)
  {
    if (name == soundname)
    {
      return mSoundFiledir + name;
    }
  }

  return "sound file not found :(";
}

std::string LoadableAssets::getSound(int which) const
{
  if (which > mSounds.size())
    return "out of range";

  return mSoundFiledir + mSounds.at(which);
}

std::vector<std::string> LoadableAssets::getAllSounds() const
{
  std::vector<std::string> pathedSounds;

  for (auto s : mSounds)
  {
    pathedSounds.push_back(mSoundFiledir + s);
  }
  
  return pathedSounds;
}

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
        mModelFiledir.clear();
        getline(ins, mModelFiledir);   // LINE 2 -- path to storage folder
        
        std::vector<std::string> all_files = get_all_files_names_within_folder(mModelFiledir);

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
        mSoundFiledir.clear();
        getline(ins, mSoundFiledir);   // LINE 2 -- path to storage folder
        getline(ins, lineBuf);  // LINE 3 -- end file type

        std::vector<std::string> all_files = get_all_files_names_within_folder(mSoundFiledir);

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
