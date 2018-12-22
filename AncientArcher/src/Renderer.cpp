#include "Renderer.h"
#include "displayManager/Display.h"
#include "Shader.h"

extern Display display;
extern Shader shader;

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::update(PrimativeManager *primativeManager) {
  display.clear();

  primativeManager->drawCube();

  display.update();
}
