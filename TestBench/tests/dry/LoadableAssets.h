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

  void getDemoConfig(std::string& return_dir, std::vector<std::string>& return_model_list, std::vector<std::string>& return_sound_list);
  //static void getSkyboxCubeMapFiles(const std::string& containingFolderPath, std::string& return_dir, std::vector<std::string>& return_file_list);
  //static void loadConfig(const std::string& pathtoconfig, std::string& return_dir, std::vector<std::string>& return_list);

private:

};

// helper functions
//static std::vector<std::string> win_GetAllFileNamesInFolder(std::string folder);
