/*
Controls
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
#include "../../include/Controls/Controls.h"
#include <GLFW\glfw3.h>
#include <memory>
#include <iostream>

namespace AA
{

float Controls::getFPPMouseSensitivity() const noexcept
{
	return mFPPMouseSensitivity;
}

void Controls::setFPPMouseSensitivity(float sensitivity) noexcept
{
	mFPPMouseSensitivity = sensitivity;
}

void Controls::perspectiveMouseMovement(float x, float y) noexcept
{
	float xOffset = 0, yOffset = 0;
	static float lastX, lastY;
	if (mRenewFPP)
	{
		mMousePosition.xOffset = 0;
		mMousePosition.yOffset = 0;
		lastX = x;
		lastY = y;
		mRenewFPP = false;
	}

	xOffset = x - lastX;
	yOffset = lastY - y;

	lastX = x;
	lastY = y;

	xOffset *= mFPPMouseSensitivity;
	yOffset *= mFPPMouseSensitivity;

	mMousePosition.xOffset = xOffset;
	mMousePosition.yOffset = yOffset;
}
/**
* Reports the mouse in x y space on the screen: bottom left should be 0,0
*/
//void Controls::standardMouseMovement(float xpos, float ypos)
//{
//	switch (mStandardMouseZeros)
//	{
//	case STANDARDMOUSEZEROS::BOT_LEFT_0_to_1:
//	{
//		const float c_xpos = xpos / Window_W;
//		const float c_ypos = -(ypos - Window_H) / Window_H;
//		mMousePosition.xOffset = c_xpos;
//		mMousePosition.yOffset = c_ypos;
//	}
//	break;
//	case STANDARDMOUSEZEROS::TOP_LEFT_0_to_1:
//	{
//		const float c_xpos = xpos / Display::getInstance()->getWindowWidth();
//		const float c_ypos = ypos / Display::getInstance()->getWindowHeight();
//		mMousePosition.xOffset = c_xpos;
//		mMousePosition.yOffset = c_ypos;
//	}
//	break;
//	case STANDARDMOUSEZEROS::BOT_LEFT_FULL_RANGE:
//	{
//		const float c_xpos = xpos;
//		const float c_ypos = -(ypos - Window_H);
//		mMousePosition.xOffset = c_xpos;
//		mMousePosition.yOffset = c_ypos;
//	}
//	break;
//	case STANDARDMOUSEZEROS::TOP_LEFT_FULL_RANGE:
//	{
//		const float c_xpos = xpos;
//		const float c_ypos = ypos;
//		mMousePosition.xOffset = c_xpos;
//		mMousePosition.yOffset = c_ypos;
//	}
//	break;
//	default:
//		std::cout << "case not handled in standard mouse zeros\n";
//	}
//}

void Controls::mouseScrollWheelMovement(float x, float y) noexcept
{
	mMouseWheelScroll.xOffset = x;
	mMouseWheelScroll.yOffset = y;
}

void Controls::resetControlVars() noexcept
{
	mFPPMouseSensitivity = 0.1f;
}

}  // end namespace AA