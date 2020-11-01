#pragma once
#include "../../../AAEngine/include/Loop.h"

static int unlit_shader = -1;

void loadUnlitShader()
{
	if (unlit_shader == -1)
		unlit_shader = LOOP->addShader("../assets/shaders/noLight.vert", "../assets/shaders/noLight.frag");
}