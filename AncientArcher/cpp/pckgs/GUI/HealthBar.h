#pragma once
#include <memory>
#include <Shader.h>
class HealthBar
{
public:
  HealthBar();
  ~HealthBar();
  void render();

  Shader* getShader();

private:
  unsigned int healthBarVAO;
  unsigned int healthBarVBO;
  unsigned int healthBarIBO;
  unsigned int indexSize;
  std::unique_ptr< Shader > healthBarShader;
};