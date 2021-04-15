#include "OGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
namespace AA {
OGLShader::OGLShader(const char* vert_src, const char* frag_src) {
  LoadShader(vert_src, frag_src);
}
OGLShader::~OGLShader() {
  glDeleteProgram(ID);
}

void OGLShader::use() const noexcept {
  glUseProgram(ID);
}

void OGLShader::stop() const noexcept {
  glUseProgram(0);
}

void OGLShader::setBool(const std::string& name, bool value) const {
  glUniform1i(getAndCheckShaderUniform(name), (int)value);
}

void OGLShader::setInt(const std::string& name, int value) const {
  glUniform1i(getAndCheckShaderUniform(name), value);
}

void OGLShader::setUint(const std::string& name, unsigned int value) const {
  glUniform1ui(getAndCheckShaderUniform(name), value);
}

void OGLShader::setFloat(const std::string& name, float value) const {
  glUniform1f(getAndCheckShaderUniform(name), value);
}

void OGLShader::setVec2(const std::string& name, const glm::vec2& value) const {
  glUniform2fv(getAndCheckShaderUniform(name), 1, &value[0]);
}

void OGLShader::setVec2(const std::string& name, float x, float y) const {
  glUniform2f(getAndCheckShaderUniform(name), x, y);
}

void OGLShader::setVec3(const std::string& name, const glm::vec3& value) const {
  glUniform3fv(getAndCheckShaderUniform(name), 1, &value[0]);
}

void OGLShader::setVec3(const std::string& name, float x, float y, float z) const {
  glUniform3f(getAndCheckShaderUniform(name), x, y, z);
}

void OGLShader::setVec4(const std::string& name, const glm::vec4& value) const {
  glUniform4fv(getAndCheckShaderUniform(name), 1, &value[0]);
}

void OGLShader::setVec4(const std::string& name, float x, float y, float z, float w) const {
  glUniform4f(getAndCheckShaderUniform(name), x, y, z, w);
}

void OGLShader::setMat2(const std::string& name, const glm::mat2& mat) const {
  glUniformMatrix2fv(getAndCheckShaderUniform(name), 1, GL_FALSE, &mat[0][0]);
}

void OGLShader::setMat3(const std::string& name, const glm::mat3& mat) const {
  glUniformMatrix3fv(getAndCheckShaderUniform(name), 1, GL_FALSE, &mat[0][0]);
}

void OGLShader::setMat4(const std::string& name, const glm::mat4& mat) const {
  glUniformMatrix4fv(getAndCheckShaderUniform(name), 1, GL_FALSE, &mat[0][0]);
}

const int OGLShader::GetID() const {
  return ID;
}

int OGLShader::getAndCheckShaderUniform(const std::string& name) const {
  const int shader_var_id = glGetUniformLocation(ID, name.c_str());

  if (shader_var_id < 0) {
    throw("shader id is invalid");
  }

  return shader_var_id;
}

/// <summary>
/// Deletes the shader program from the GPU. The Shader is no longer usable after calling this.
/// </summary>
//void OGLShader::deleteShader() {
//  glDeleteProgram(ID);
//}

void OGLShader::LoadShader(const char* vert_source, const char* frag_source) {
  /* compile vertex (points) shader */
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vert_source, nullptr);
  glCompileShader(vertexShader);

  /* determine if vertex shader was successful in compiling */
  int v_success;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
  if (!v_success) {
    char v_infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
    std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* compile fragment shader */
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &frag_source, nullptr);
  glCompileShader(fragmentShader);

  /* determine if fragment shader was successful in compiling */
  int f_success;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
  if (!f_success) {
    char f_infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, f_infoLog);
    std::cout << "error in fragment shader, compilation failed: " << f_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* make the shader program */
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  /* check that the ID was successful */
  int p_success;
  glGetProgramiv(ID, GL_LINK_STATUS, &p_success);
  if (!p_success) {
    char p_infoLog[512];
    glGetProgramInfoLog(ID, 512, nullptr, p_infoLog);
    std::cout << "error in ID: " << p_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* we don't need them anymore */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

}  // end namespace AA