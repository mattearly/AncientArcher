#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char *vertex_file, const char *fragment_file) {

  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {

    vShaderFile.open(vertex_file);
    fShaderFile.open(fragment_file);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vertexShaderSource = vertexCode.c_str();
  const char *fragmentShaderSource = fragmentCode.c_str();

  /* compile vertex (points) shader */
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  /* determine if vertex shader was successful in compiling */
  int v_success;
  char v_infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
  if (!v_success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
    std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* compile fragment shader */
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  /* determine if fragment shader was successful in compiling */
  int f_success;
  char f_infoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
  if (!f_success) {
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
  char p_infoLog[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &p_success);
  if (!p_success) {
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

Shader::Shader(const char * vertex_file, const char * fragment_file, const char * geometry_file) {

  std::string vertexCode;
  std::string fragmentCode;
  std::string geometryCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;
  std::ifstream gShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {

    vShaderFile.open(vertex_file);
    fShaderFile.open(fragment_file);
    gShaderFile.open(geometry_file);

    std::stringstream vShaderStream, fShaderStream, gShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    gShaderStream << gShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();
    gShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    geometryCode = gShaderStream.str();

  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const char *vertexShaderSource = vertexCode.c_str();
  const char *fragmentShaderSource = fragmentCode.c_str();
  const char *geometryShaderSource = geometryCode.c_str();

  /* compile vertex (points) shader */
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  /* determine if vertex shader was successful in compiling */
  int v_success;
  char v_infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &v_success);
  if (!v_success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, v_infoLog);
    std::cout << "error in vertex shader, compilation failed: " << v_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* compile fragment shader */
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  /* determine if fragment shader was successful in compiling */
  int f_success;
  char f_infoLog[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &f_success);
  if (!f_success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, f_infoLog);
    std::cout << "error in fragment shader, compilation failed: " << f_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* compile geometry shader */
  int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
  glCompileShader(geometryShader);

  /* determine if geometry shader was successful in compiling */
  int g_success;
  char g_infoLog[512];
  glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &g_success);
  if (!g_success) {
    glGetShaderInfoLog(geometryShader, 512, nullptr, g_infoLog);
    std::cout << "error in geometry shader, compilation failed: " << g_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* make the shader program */
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glAttachShader(ID, geometryShader);
  glLinkProgram(ID);

  /* check that the ID was successful */
  int p_success;
  char p_infoLog[512];
  glGetProgramiv(ID, GL_LINK_STATUS, &p_success);
  if (!p_success) {
    glGetProgramInfoLog(ID, 512, nullptr, p_infoLog);
    std::cout << "error in ID: " << p_infoLog << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  /* we don't need them anymore */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(geometryShader);


}

Shader::~Shader() {}

void Shader::use() const {
  glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUint(const std::string & name, unsigned int value) const {
  glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string & name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string & name, glm::vec2 & value) const {
  glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string & name, float x, float y) const {
  glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string & name, glm::vec3 & value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string & name, float x, float y, float z) const { 
  glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string & name, glm::vec4 & value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string & name, float x, float y, float z, float w) const {
  glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string & name, const glm::mat2 & mat) const {
  glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string & name, const glm::mat3 & mat) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string & name, const glm::mat4 & mat) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
