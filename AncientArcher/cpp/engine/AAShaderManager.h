#pragma once
#include <memory>
#include <mearly\Shader.h>
#include <map>

class AAShaderManager
{
public:

  int addShader(const char* vertfile, const char* fragfile);
  void updateViewMatrices();
  void updateProjectionMatrices();

  const Shader& getShader(int id) const;

private:

  std::map<int, Shader> mShaders;

};