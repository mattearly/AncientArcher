/*
  Stores a string list of your asset files. Based on config file that must be parsed.
  The goal is to have an automatic setup for updating your list of usable assets.
*/
#pragma once;
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

class LoadableAssets {
public:

  static void loadDemoConfig(std::string& return_dir, std::vector<std::string>& model_return_list, std::vector<std::string>& sound_return_list);
  //static void loadConfig(const std::string& pathtoconfig, std::string& return_dir, std::vector<std::string>& return_list);

private:

};

// helper functions
//static std::vector<std::string> win_GetAllFileNamesInFolder(std::string folder);
static std::vector<std::filesystem::path> cpp17_GetAllFileNamesInFolder(std::filesystem::path path);
