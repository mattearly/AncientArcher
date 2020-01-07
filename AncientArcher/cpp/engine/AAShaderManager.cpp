#include "AAShaderManager.h"
#include "../engine/AAViewport.h"
#include <utility>

void AAShaderManager::setPath(std::string path)
{
  mPath = path;
}

void AAShaderManager::clearPath()
{
  mPath.clear();
}

int AAShaderManager::addShader(const char* vertfile, const char* fragfile)
{
  static int shaderManagerID = 0;
  shaderManagerID++;
  if (!mPath.empty())
  {
    std::string pathedVertfile = mPath + vertfile;
    std::string pathedFragfile = mPath + fragfile;
    std::pair<int, Shader> newPair = std::make_pair(shaderManagerID, Shader{ pathedVertfile.c_str(), pathedFragfile.c_str() });
    mShaders.emplace(newPair);
    return shaderManagerID;
  }
  else
  {
    std::pair<int, Shader> newPair = std::make_pair(shaderManagerID, Shader{ vertfile, fragfile });
    mShaders.emplace(newPair);
    return shaderManagerID;
  }
}

void AAShaderManager::updateViewMatrices()
{
  for (auto s : mShaders)
  {
    AAViewport::getInstance()->updateViewMatrix(s.second);
  }
}

void AAShaderManager::updateProjectionMatrices()
{
  for (auto s : mShaders)
  {
    AAViewport::getInstance()->updateProjectionMatrix(s.second);
  }
}

const Shader& AAShaderManager::getShader(int id) const
{
  return mShaders.at(id);
}
