#include "../../../include/Window/Display/DisplayCallbacks.h"
#include "../../../include/AncientArcher.h"

namespace AA
{
void DisplayCallbacks::perspectiveMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine->PerspectiveMouseHandler(window, (float)xpos, (float)ypos);
}
void DisplayCallbacks::standardMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine->StandardMouseHandler(window, (float)xpos, (float)ypos);
}
void DisplayCallbacks::scrollCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine->ScrollHandler(window, (float)xpos, (float)ypos);
}
void DisplayCallbacks::reshapeCallback(GLFWwindow* window, int w, int h)
{
	Engine->ReshapeWindowHandler(window, w, h);
}

}  // end namespace AA