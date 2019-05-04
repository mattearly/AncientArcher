#pragma once
#include <glm/glm.hpp>
#include <vector>
class Shader;
class TextureLoader;
class Skybox
{
public:
  Skybox();
  ~Skybox();
  
  void loadProjectionMatrix(glm::mat4 projectionMatrix);
  void loadViewMatrix();

private:
  Shader* skyboxShader;
  TextureLoader* loader;
};
