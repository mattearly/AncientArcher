#include "Game.h"
#include "Controls.h"
#include "Display.h"
#include <glm/glm.hpp>

//Camera camera;  // default

Camera camera(
  glm::vec3(10.f, 9.f, 11.f),   // starting Position
  -90.f,                          // YAW
  -7.5f,                          // PITCH
  80.f                           // FoV
);

//Display display; // default - fpp

Display display("SSFighter", Display::MouseControlType::SIDESCROLLER);

int main()
{

  Game game;

  game.mainLoop();

  return 0;

}
