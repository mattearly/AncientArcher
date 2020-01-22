#include "LoadableAssets.h"
#include <iostream>

std::string LoadableAssets::get(std::string objname)
{
  if (mFilenames.empty())
    return "no filenames :(";

  for (auto name : mFilenames)
  {
    if (name == objname)
    {
      return mFiledir + name;
    }
  }

  return "file not found :(";
}

std::string LoadableAssets::get(int which)
{
  if (which > mFilenames.size())
    return "out of range";

  return mFiledir + mFilenames.at(which);
}

std::string LoadableAssets::operator[](int which)
{
  if (which > mFilenames.size())
    return "out of range";

  return mFilenames.at(which);
}

void LoadableAssets::updateListOfModelsFromConfig(std::string pathtoconfig)
{
  {

    // open file
    std::ifstream ins(pathtoconfig);
    if (ins.is_open())
    {
      // clear both lists
      mFilenames.clear();
      mFiledir.clear();

      // parse
      std::string aLine;
      // get path
      getline(ins, aLine);
      mFiledir = aLine;

      // debug: show filedir
      std::cout << "filedir: " << mFiledir << '\n';

      // get the model paths
      do
      {
        getline(ins, aLine);
        if (aLine.size() > 1)
        {
          mFilenames.push_back(aLine);
        }

      } while (!ins.eof());

      // debug: show all the models
      std::cout << "all the models:\n\n";
      for (auto name : mFilenames)
      {
        std::cout << '\t' << name << '\n';
      }
    }
    else
    {
      std::cout << "failed to open your config file @ \n\t" << pathtoconfig << '\n';
    }
  }
}
