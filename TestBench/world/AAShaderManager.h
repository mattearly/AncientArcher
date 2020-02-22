#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <string>

class Shader
{
public:

  unsigned int ID;

  Shader(const char* vertex_file, const char* fragment_file);
  Shader(const char* vertex_file, const char* fragment_file, const char* geometry_file);

  void use() const;
  void stop() const;
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setUint(const std::string& name, unsigned int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec2(const std::string& name, glm::vec2& value) const;
  void setVec2(const std::string& name, float x, float y) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void setVec4(const std::string& name, glm::vec4& value) const;
  void setVec4(const std::string& name, float x, float y, float z, float w) const;
  void setMat2(const std::string& name, const glm::mat2& mat) const;
  void setMat3(const std::string& name, const glm::mat3& mat) const;
  void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
  Shader() = delete;

};



class AAShaderManager
{
public:

  void setPath(std::string path);

  int addShader(const char* vertfile, const char* fragfile);
  void updateViewMatrices();
  void updateProjectionMatrices();

  const Shader& getShader(int id) const;

private:
  std::string mPath;
  std::map<int, Shader> mShaders;

  void clearPath() noexcept;

};