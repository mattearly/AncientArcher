/*
  Stores a string list of your asset files. Based on config file that must be parsed.
  The goal is to have an automatic setup for updating your list of usable assets.
*/
#pragma once;
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string> get_all_files_names_within_folder(std::string folder);

class LoadableAssets {
public:

  std::string getModel(std::string objname) const;
  std::string getModel(int which) const;
  std::vector<std::string> getAllModels() const;
  std::size_t getNumLoadableModels() const noexcept;

  std::string getSound(std::string soundname) const;
  std::string getSound(int which) const;
  std::vector<std::string> getAllSounds() const;
  std::size_t getNumLoadableSounds() const noexcept;

  void loadConfig(std::string pathtoconfig);
  void loadDemoConfig();

private:

  std::string mModelFileDir;
  std::vector<std::string> mModels;

  std::string mSoundFileDir;
  std::vector<std::string> mSounds;

};
