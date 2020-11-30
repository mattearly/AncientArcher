#pragma once
#include "../../../AAEngine/include/Loop.h"

extern int playerCam;
extern int unlit_shader;
static int playerId = -1;

// for our move speed controls
static const float DEFAULTMOVESPEED = 10.f;
static const float MAXSPEED = 400.f;
static const float FLYINCR = 1.f;

void putPlayerInWorld()
{
	playerId = LOOP->addObject("../assets/models/fbx/Ch18_nonPBR.fbx", playerCam, unlit_shader);
	// for our move direction and speed
	static float currFlySpeed = DEFAULTMOVESPEED;
	static float prevFlySpeed = currFlySpeed;
	static glm::vec3 moveDir = glm::vec3(0.f);
	static glm::vec3 frontFacingDir = glm::vec3(*LOOP->getCamera(playerCam).getFront());
	static glm::vec3 rightFacingDir = glm::vec3(*LOOP->getCamera(playerCam).getRight());

	// add WASD key first person movement function
	const auto wasd = [](AA::KeyboardInput& key) {
		if (key.w)
		{
			moveDir += frontFacingDir;
		}
		else if (key.s)
		{
			moveDir -= frontFacingDir;
		}
		if (key.a)
		{
			moveDir -= rightFacingDir;
		}
		else if (key.d)
		{
			moveDir += rightFacingDir;
		}
	};
	LOOP->addToKeyHandling(wasd);

	const auto camMove = [](float dt) {
		static float frameCalculatedVelocity = 0.f;
		frameCalculatedVelocity = dt * currFlySpeed;
		LOOP->getCamera(playerCam).shiftCurrentPosition(moveDir * frameCalculatedVelocity);
		moveDir = glm::vec3(0.f);
		frontFacingDir = glm::vec3(*LOOP->getCamera(playerCam).getFront());
		rightFacingDir = glm::vec3(*LOOP->getCamera(playerCam).getRight());
		LOOP->getCamera(playerCam).updateCameraVectors();
	};
	LOOP->addToDeltaUpdate(camMove);

	// add mouse movement to change our view direction
	const auto mouselook = [](AA::MouseInput& cursor)
	{
		LOOP->getCamera(playerCam).shiftYawAndPitch(cursor.xOffset, cursor.yOffset);
	};
	LOOP->addToMouseHandling(mouselook);

	// add mouse scroll wheel to change fly speed
	const auto mousewheelflyspeed = [](AA::ScrollInput& wheel)
	{
		// set flyspeed when mouse wheel moves
		if (wheel.yOffset > 0.1f)
		{
			currFlySpeed += FLYINCR;
			wheel.yOffset = 0;
		}
		else if (wheel.yOffset < -0.1f)
		{
			currFlySpeed -= FLYINCR;
			wheel.yOffset = 0;
		}

		// cap flyspeed
		if (currFlySpeed >= MAXSPEED)
		{
			currFlySpeed = MAXSPEED;
		}
		else if (currFlySpeed <= 1.f)
		{
			currFlySpeed = 1.000001f;
		}

		// set flyspeed if it changed
		if (currFlySpeed != prevFlySpeed)
		{
			prevFlySpeed = currFlySpeed;
		}
	};
	LOOP->addToScrollHandling(mousewheelflyspeed);
}