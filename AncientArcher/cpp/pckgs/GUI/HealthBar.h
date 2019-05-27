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

  void setBar1(unsigned int current);

  void setBar2(unsigned int current);

;

private:
  unsigned int healthBarVAO;
  unsigned int healthBarVBO;
  unsigned int healthBarIBO;
  unsigned int indexSize;
  unsigned int bar1current;
  unsigned int bar2current;
  std::unique_ptr< Shader > healthBarShader;
};