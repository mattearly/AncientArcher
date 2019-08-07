#include "Game.h"
#include "Display.h"

Display g_display("AADemo", 640, 640, false);

int main()
{
  
  Game game;

  game.mainLoop();

  return 0;

}
