#include "Game.h"
#include "Controls.h"
#include "Display.h"
#include <glm/glm.hpp>

//Camera camera;  // default

Camera camera(glm::vec3(13.f, 3.f, -10.f), 10.f, 16.f);

//Display display; // default - fpp

Display display("SSFighter", Display::MouseControlType::SIDESCROLLER);

int main()
{

  Game game;

  game.mainLoop();

  return 0;

}
