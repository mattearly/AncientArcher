#include "RPG.h"
int RPG::init()
{
	gamestate = RPGSTATE::run;
	return 0;
}
int RPG::run()
{
	init();
	return 0;
}