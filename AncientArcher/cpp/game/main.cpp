#include "Game.h"
#include "Display.h"

Display g_display("AADemo");

int main()
{

  Game game;

  game.mainLoop();

  return 0;

}
