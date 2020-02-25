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

  void use() const noexcept;
  void stop() const noexcept;
  void setBool(const std::string& name, bool value) const noexcept;
  void setInt(const std::string& name, int value) const noexcept;
  void setUint(const std::string& name, unsigned int value) const noexcept;
  void setFloat(const std::string& name, float value) const noexcept;
  void setVec2(const std::string& name, glm::vec2& value) const noexcept;
  void setVec2(const std::string& name, float x, float y) const noexcept;
  void setVec3(const std::string& name, const glm::vec3& value) const noexcept;
  void setVec3(const std::string& name, float x, float y, float z) const noexcept;
  void setVec4(const std::string& name, glm::vec4& value) const noexcept;
  void setVec4(const std::string& name, float x, float y, float z, float w) const noexcept;
  void setMat2(const std::string& name, const glm::mat2& mat) const noexcept;
  void setMat3(const std::string& name, const glm::mat3& mat) const noexcept;
  void setMat4(const std::string& name, const glm::mat4& mat) const noexcept;

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