#pragma once
#include <glm/glm.hpp>
class AAViewport
{
public:

  void setToOrtho();
  void setToPerspective();

private:

  glm::mat4 mDisplayMatrix;

  //Shader
  //Camera

};

