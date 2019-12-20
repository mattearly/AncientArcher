#include "AAShaderManager.h"
#include "../engine/AAViewport.h"
#include <utility>

int AAShaderManager::addShader(const char* vertfile, const char* fragfile)
{
  static int shaderManagerID = 0;
  shaderManagerID++;
  std::pair<int, Shader> newPair = std::make_pair(shaderManagerID, Shader{ vertfile, fragfile });
  mShaders.emplace(newPair);
  return shaderManagerID;
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
