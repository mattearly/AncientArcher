#include "Controls.h"
#include "../../Renderer/OpenGL/OGLGraphics.h"
#include <memory>
#include <GLFW/glfw3.h>
#include "../../../include/AncientArcher/AncientArcher.h"

namespace AA
{

extern bool WindowSizeDirty;

float Controls::GetMouseFPPSensitivity() const noexcept
{
	return mFPPMouseSensitivity;
}

void Controls::SetMouseFPPSensitivity(float sensitivity) noexcept
{
	mFPPMouseSensitivity = sensitivity;
}

void Controls::PerspectiveMouseHandler(float xpos, float ypos)
{
	perspectiveMouseMovement(xpos, ypos);
}

void Controls::StandardMouseHandler(float xpos, float ypos)
{
	standardMouseMovement(xpos, ypos);
}

void Controls::ScrollHandler(float xpos, float ypos)
{
	mouseScrollWheelMovement(xpos, ypos);
}

void Controls::resetControlVars() noexcept
{
	mFPPMouseSensitivity = 0.1f;
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
void Controls::standardMouseMovement(float xpos, float ypos)
{
	float DisplayWindowWidth = static_cast<float>(Engine->GetWindowWidth());
	float DisplayWindowHeight = static_cast<float>(Engine->GetWindowHeight());

	switch (mStandardMouseZeros)
	{
	case STANDARDMOUSEZEROS::BOT_LEFT_0_to_1:
	{
		const float c_xpos = xpos / DisplayWindowWidth;
		const float c_ypos = -(ypos - DisplayWindowHeight) / DisplayWindowHeight;
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	case STANDARDMOUSEZEROS::TOP_LEFT_0_to_1:
	{
		const float c_xpos = xpos / DisplayWindowWidth;
		const float c_ypos = ypos / DisplayWindowHeight;
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	case STANDARDMOUSEZEROS::BOT_LEFT_FULL_RANGE:
	{
		const float c_xpos = xpos;
		const float c_ypos = -(ypos - DisplayWindowHeight);
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	case STANDARDMOUSEZEROS::TOP_LEFT_FULL_RANGE:
	{
		const float c_xpos = xpos;
		const float c_ypos = ypos;
		mMousePosition.xOffset = c_xpos;
		mMousePosition.yOffset = c_ypos;
	}
	break;
	}
}
void Controls::mouseScrollWheelMovement(float x, float y) noexcept
{
	mMouseWheelScroll.xOffset = x;
	mMouseWheelScroll.yOffset = y;
}

}  // end namespace AA