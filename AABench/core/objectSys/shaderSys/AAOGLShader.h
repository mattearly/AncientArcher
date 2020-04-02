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

  //uniform setters
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setUint(const std::string& name, unsigned int value) const;
  void setFloat(const std::string& name, float value) const;

  void setVec2(const std::string& name, const glm::vec2& value) const;
  void setVec2(const std::string& name, float x, float y) const;

  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;

  void setVec4(const std::string& name, const glm::vec4& value) const;
  void setVec4(const std::string& name, float x, float y, float z, float w) const;

  void setMat2(const std::string& name, const glm::mat2& mat) const;
  void setMat3(const std::string& name, const glm::mat3& mat) const;
  void setMat4(const std::string& name, const glm::mat4& mat) const;

  const int getID() const;

  int getAndCheckShaderUniform(const std::string& name) const;

private:
  AAOGLShader() = delete;

  unsigned int ID;

};

