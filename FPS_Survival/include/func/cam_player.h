#pragma once
#include "../../../AAEngine/include/Loop.h"

static int playerCam = -1;

void setPlayerCam()
{
	playerCam = LOOP->addCamera();
	LOOP->getCamera(playerCam).setCurrentPosition(glm::vec3(0, -96, 0));
	// config engine loop for first person perspective view and controls
	LOOP->setCursorToDisabled();
	LOOP->setToPerspectiveMouseHandling();
}