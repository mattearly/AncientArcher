#pragma once
#include "../../../AAEngine/include/Loop.h"

static int playerCam = -1;

void setPlayerCam()
{
	playerCam = LOOP->addCamera();
	// config engine loop for first person perspective view and controls
	LOOP->setCursorToDisabled();
	LOOP->setToPerspectiveMouseHandling();

}