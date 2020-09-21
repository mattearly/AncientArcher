/*
addGodMovements
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
	copyright notice, this list of conditions and the
	following disclaimer.
* Redistributions in binary form must reproduce the above
	copyright notice, this list of conditions and the
	following disclaimer in the documentation and/or other
	materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
	contributors may be used to endorse or promote products
	derived from this software without specific prior
	written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#pragma once
#include <iostream>
#include "Loop.h"
#include "WorldUp.h"

#define UP AA::WorldUp::getWorldUp()

static constexpr float startingSpeed = 10.5f;
static constexpr float MAXFLYSPEED = 400.f;
static constexpr float FlyIncrement = 1.f;

/**
 * setGodCamWithMovement is for setting up defaults and movement commands on a First Person Perspective Camera.
 * * After calling this, the camera view will be able to fly freely.
 * Hotkeys:
 * * MOUSE          :  look
 * * WASD           :  move
 * * SPACE          :  up
 * * LSHIFT+SPACE   :  down
 * * MOUSEWHEEL+/-  :  change move speed
 * * TAB            :  toggle mouse cursor mode
*/
int setGodCamWithMovement()
{
	LOOP->setCursorToDisabled();
	LOOP->setToPerspectiveMouseHandling();

	static int mainCamId;
	mainCamId = LOOP->addCamera();

	static float currFlySpeed = startingSpeed;
	static float prevFlySpeed = startingSpeed;

	// allows wasd too move you around basedon the direction the cam is facing
	const auto flying_wasd = [](AA::KeyboardInput& keys)
	{
		// this is a debug cam mover with no colliding
		static float fps60velocity = 0.f;
		static glm::vec3 directionPlacement = glm::vec3(0.f, 0.f, 0.f);
		static glm::vec3 moveFront = glm::vec3(*LOOP->getCamera(mainCamId).getFront());

		// setting our velocity based on 60fps (a guess since we don't have delta time here)
		fps60velocity = 0.0166f * currFlySpeed;

		// process WASD movement
		if (keys.w)
		{
			directionPlacement += moveFront * fps60velocity;
		}
		if (keys.s)
		{
			directionPlacement -= moveFront * fps60velocity;
		}
		if (keys.a)
		{
			directionPlacement -= *LOOP->getCamera(mainCamId).getRight() * fps60velocity;
		}
		if (keys.d)
		{
			directionPlacement += *LOOP->getCamera(mainCamId).getRight() * fps60velocity;
		}

		// process going up and down
		if (!keys.leftShift && keys.spacebar)
		{
			directionPlacement += UP * fps60velocity;
		}
		if (keys.leftShift && keys.spacebar)
		{
			directionPlacement -= UP * fps60velocity;
		}

		LOOP->getCamera(mainCamId).shiftCurrentPosition(directionPlacement);

		// reset local variables for next frame processing
		directionPlacement = glm::vec3(0.f, 0.f, 0.f);
		moveFront = glm::vec3(*LOOP->getCamera(mainCamId).getFront());
	};
	LOOP->addToKeyHandling(flying_wasd);

	// allows mouse movement to change the cam look direction while the mouse cursor is disabled
	const auto mouse_movement_look_while_cursor_disabled = [](AA::MouseInput& mouse)
	{
		if (LOOP->getMouseReportingMode() == AA::MouseReporting::PERSPECTIVE)
		{
			LOOP->getCamera(mainCamId).shiftYawAndPitch(mouse.xOffset, mouse.yOffset);
		}
		else // if (mainWorld.getMouseReportingMode() == MouseReporting::STANDARD)
		{
			// dont change the cam location
		}
	};
	LOOP->addToMouseHandling(mouse_movement_look_while_cursor_disabled);

	// allows mouse scroll wheel to adjust move speed
	const auto mouse_scroll_changes_wasd_speed = [](AA::ScrollInput& scroll)
	{
		// set flyspeed when mouse wheel moves
		if (scroll.yOffset > 0.1f)
		{
			currFlySpeed += FlyIncrement;
			scroll.yOffset = 0;
		}
		if (scroll.yOffset < -0.1f)
		{
			currFlySpeed -= FlyIncrement;
			scroll.yOffset = 0;
		}
		// cap flyspeed
		if (currFlySpeed >= MAXFLYSPEED)
		{
			currFlySpeed = MAXFLYSPEED;
		}
		if (currFlySpeed <= 1.f)
		{
			currFlySpeed = 1.000001f;
		}
		if (currFlySpeed != prevFlySpeed)
		{
			prevFlySpeed = currFlySpeed;
		}
	};
	LOOP->addToScrollHandling(mouse_scroll_changes_wasd_speed);

	// allows tab to toggle the cursor from disabled(fpp movement mode) or visible(inventory mode)
	const auto tab_toggles_cursor_disabled_or_visible = [](AA::KeyboardInput& keys)
	{
		if (keys.tab)
		{
			if (LOOP->getMouseReportingMode() == AA::MouseReporting::PERSPECTIVE)
			{
				// show the mouse
				LOOP->setCursorToEnabled();
				// change to a new reporting method
				LOOP->setToStandardMouseHandling();

				// turn the key handling to not move the camera around, preserving cam  look the cam look
				// todo
			}
			else  // it is in standard calc, toggle to perspective
			{
				// dont show the mouse and change it to first person live control
				LOOP->setCursorToDisabled();
				// set to perspective view handling which reports the mouse
				LOOP->setToPerspectiveMouseHandling();
			}
			return true;
		}
		return false;
	};
	LOOP->addToTimedOutKeyHandling(tab_toggles_cursor_disabled_or_visible);

	return mainCamId;
}