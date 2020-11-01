#pragma once

struct GameSettings
{
	float RenderDistance = 1000.f;
};

static GameSettings* settings = new GameSettings();

extern int playerCam;

void setGameSettings(const GameSettings* settings)
{
	if (playerCam == -1)
		return;

	LOOP->setRenderDistance(playerCam, settings->RenderDistance);

}
