#pragma once
#include "displayManager/Display.h"
#include "primatives/PrimativeManager.h"
#include "Shader.h"
class Renderer {
public:
  Renderer();
  ~Renderer();
  void update(const Display *display, const Shader *shader, PrimativeManager *primativeManager);

};

