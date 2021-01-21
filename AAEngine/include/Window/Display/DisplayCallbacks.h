#pragma once
#include "Display.h"
namespace AA
{
class DisplayCallbacks 
{
friend class Display;
private:
	static void perspectiveMouseCallback(GLFWwindow * window, double xpos, double ypos);
	static void standardMouseCallback(GLFWwindow * window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow * window, double xpos, double ypos);
	static void reshapeCallback(GLFWwindow * window, int w, int h);
};
}  //end namespace AA