#include "Game.h"
#include "Controls.h"
#include "Display.h"
#include <glm/glm.hpp>

//Camera camera;  // default

Camera camera(
  glm::vec3(12.f, 9.f, 13.f),   // starting Position
  -88.f,                          // YAW
  -7.5f,                          // PITCH
  65.f                           // FoV
);

//Display display; // default - fpp

Display display("SSFighter", Display::MouseControlType::SIDESCROLLER);

int main()
{

  Game game;

  game.mainLoop();

  return 0;

}
