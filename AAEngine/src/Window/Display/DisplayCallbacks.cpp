#include "DisplayCallbacks.h"
#include "../../../include/AncientArcher/AncientArcher.h"

namespace AA
{
void DisplayCallbacks::perspectiveMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine->PerspectiveMouseHandler(static_cast<float>(xpos), static_cast<float>(ypos));
}
void DisplayCallbacks::standardMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine->StandardMouseHandler(static_cast<float>(xpos), static_cast<float>(ypos));
}
void DisplayCallbacks::scrollCallback(GLFWwindow* window, double xpos, double ypos)
{
	Engine->ScrollHandler(static_cast<float>(xpos), static_cast<float>(ypos));
}
void DisplayCallbacks::reshapeCallback(GLFWwindow* window, int w, int h)
{
	Engine->ReshapeWindowHandler(w, h);
}

}  // end namespace AA