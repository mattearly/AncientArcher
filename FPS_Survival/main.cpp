#include "include/Survivor.h"
#include "../AAEngine/include/Loop.h"
#include "include/func/skybox_storm.h"
#include "include/func/cam_player.h"
#include "include/func/load_world.h"
#include "include/func/player_to_world.h"
#include "include/func/game_settings.h"


int main()
{
	setPlayerCam();
	setStormySkybox();
	loadWorld();
	putPlayerInWorld();



	setGameSettings(settings);
	return LOOP->runMainLoop();
}