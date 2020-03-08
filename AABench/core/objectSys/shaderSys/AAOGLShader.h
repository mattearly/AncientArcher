#pragma once
#include <string>
#include <ColliderBox.h>
class AAOGLShader
{
public:


  AAOGLShader(const char* vertex_file, const char* fragment_file);
  AAOGLShader(const char* vertex_file, const char* fragment_file, const char* geometry_file);

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

  const int getID() const;

  int getAndCheckShaderUniform(const std::string& name) const noexcept;

private:
  AAOGLShader() = delete;

  unsigned int ID;

};

