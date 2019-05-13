#include "Game.h"
#include "Controls.h"
#include "Display.h"

Controls controls;

Display display;

int main()
{

  Game game;

  game.mainLoop();

  return 0;

}
