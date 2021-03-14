#pragma once
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "ScrollInput.h"

struct GLFWwindow;
namespace AA
{
enum class STANDARDMOUSEZEROS { DEFAULT, TOP_LEFT_0_to_1, BOT_LEFT_0_to_1, TOP_LEFT_FULL_RANGE, BOT_LEFT_FULL_RANGE };

class Controls
{
public:

	float GetMouseFPPSensitivity() const noexcept;
	void SetMouseFPPSensitivity(float sensitivity) noexcept;

	void PerspectiveMouseHandler(float xpos, float ypos);
	void StandardMouseHandler(float xpos, float ypos);
	void ScrollHandler(float xpos, float ypos);

protected:

	void perspectiveMouseMovement(float xpos, float ypos) noexcept;
	void standardMouseMovement(float xpos, float ypos);
	void mouseScrollWheelMovement(float xpos, float ypos) noexcept;

	void resetControlVars() noexcept;

	KeyboardInput mButtonState = {};
	MouseInput    mMousePosition = {};
	ScrollInput   mMouseWheelScroll = {};
	bool mRenewFPP = true;

private:

	STANDARDMOUSEZEROS mStandardMouseZeros = STANDARDMOUSEZEROS::BOT_LEFT_0_to_1;

	float mFPPMouseSensitivity = 0.1f;  ///< mouse sensitivity while in first person perspective

};
}  // end namespace AA