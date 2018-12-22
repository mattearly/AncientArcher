#include "Renderer.h"
#include "displayManager/Display.h"
#include "Shader.h"
#include "TextureBank.h"
#include "Constraints.h"

extern Display display;
extern Shader shader;
extern TextureBank texBank;

Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::update(PrimativeManager *primativeManager, float deltaTime) {
  display.clear();

  texBank.activate(0);    //floor
  primativeManager->drawCube(glm::vec3(world_width / 2.0f, 0.0f, world_width / 2.0f), glm::vec3(1000.0f, 0.01f, 1000.0f)); 
  

  //east/west  ??
  texBank.activate(11);    // WALLS 1
  primativeManager->drawCube(glm::vec3(world_width / 2.0f, world_width / 8.0f, 0.0f), glm::vec3(1000.0f, 250.0f, 0.01f));
  texBank.activate(10);    //2
  primativeManager->drawCube(glm::vec3(world_width / 2.0f, world_width / 8.0f, world_width), glm::vec3(1000.0f, 250.0f, 0.01f));
  
  //north/south ??
  texBank.activate(7);    //3
  primativeManager->drawCube(glm::vec3(0.0f, world_width / 8.0f, world_width / 2.0f), glm::vec3(0.01f, 250.0f, 1000.0f));
  texBank.activate(4);    //4
  primativeManager->drawCube(glm::vec3(world_width, world_width / 8.0f, world_width / 2.0f), glm::vec3(0.01f, 250.0f, 1000.0f));

  display.update();

}
