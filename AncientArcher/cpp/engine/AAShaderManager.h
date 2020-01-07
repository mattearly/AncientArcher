#pragma once
#include <memory>
#include <mearly\Shader.h>
#include <map>

class AAShaderManager
{
public:

  void setPath(std::string path);
  void clearPath();

  int addShader(const char* vertfile, const char* fragfile);
  void updateViewMatrices();
  void updateProjectionMatrices();

  const Shader& getShader(int id) const;

private:
  std::string mPath;
  std::map<int, Shader> mShaders;

};