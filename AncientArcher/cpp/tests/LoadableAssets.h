/*
  Stores a list of your files. Based on config files @ config/*.txt
  The goal is to have an automatic setup for updating your list of usable assets.
*/
#pragma once;
#include <fstream>
#include <vector>
#include <string>

class LoadableAssets {
public:

  std::string getModel(std::string objname) const;
  std::string getModel(int which) const;

  std::string getSound(std::string soundname) const;
  std::string getSound(int which) const;

  void loadConfig(std::string pathtoconfig);

private:

  std::string mModelFiledir;
  std::vector<std::string> mModels;

  std::string mSoundFiledir;
  std::vector<std::string> mSounds;

};
