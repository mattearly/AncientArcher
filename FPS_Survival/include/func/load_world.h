#pragma once
#include "load_shader.h"

extern int playerCam;

void loadWorld()
{
	if (unlit_shader == -1)
		loadUnlitShader();

	if (playerCam == -1)
		return;

	// add a grass plane collada object
	static int grassyOneId = LOOP->addObject("../assets/models/dae/grassy1.dae", playerCam, unlit_shader);
	LOOP->getGameObject(grassyOneId).translateTo(glm::vec3(0.f, -100.f, 0.f));

}