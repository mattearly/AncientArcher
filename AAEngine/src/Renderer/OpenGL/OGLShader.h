#pragma once
#include <string>
#include <glm\glm.hpp>
namespace AA
{
class OGLShader
{
public:

  OGLShader(const char* vert_src, const char* frag_src);
  ~OGLShader();

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

  const int GetID() const;

  int getAndCheckShaderUniform(const std::string& name) const;

  //void deleteShader();

private:
  OGLShader() = delete;

  unsigned int ID;

  void LoadShader(const char* vert, const char* frag);
};
}  // end namespace AA