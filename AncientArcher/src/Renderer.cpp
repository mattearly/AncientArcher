#include "Renderer.h"

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::update(const Display *display, const Shader *shader, PrimativeManager *primativeManager) {
  display->clear();


  primativeManager->drawCube(shader);



  display->update();
}
