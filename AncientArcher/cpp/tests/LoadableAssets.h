/*
  Loads the models from your config file @ config/models.txt
  The goal is to have an easier setup for updating your list of assets
*/
#pragma once;
#include <fstream>
#include <vector>
#include <string>

class LoadableAssets {
public:

  std::string get(std::string objname);
  std::string get(int which);
  std::string operator[] (int which);

  void updateListOfModelsFromConfig(std::string pathtoconfig);

private:

  std::vector<std::string> mFilenames;

  std::string mFiledir;
};
