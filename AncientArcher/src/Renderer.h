#pragma once
#include "primatives/PrimativeManager.h"

class Renderer {
public:
  Renderer();
  ~Renderer();

  void enableGLBlend();
  void disableGLBlend();

  void enableGLDepthTest();
  void disableGLDepthTest();

  void update(PrimativeManager *primativeManager, float deltaTime);

};

