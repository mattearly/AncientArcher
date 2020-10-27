/*
DisplayCallbacks
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
// ---------------
// GLFW3 callbacks
// ---------------
#include "../include/Display.h"
#include "../include/Controls.h"
//#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
namespace AA
{
//---------------------------------------------------------------------------------------
// RESHAPE WINDOW CALLBACK SETUP
void Display::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	glViewport(0, 0, mWindowWidth, mWindowHeight);

	//AAViewport::getInstance()->windowViewportChanged();  //todo: make this work with context viewports  -- see below

	// engine checks this mWindowSizeChanged var and does stuff, changing mWindowSizeChanged to false when done
	mWindowSizeChanged = true;
}

extern "C" void reshapeCallback(GLFWwindow * window, int w, int h)
{
	Display::getInstance()->reshapeWindowHandler(window, w, h);
}

void Display::setReshapeWindowHandler() noexcept
{
	::glfwSetFramebufferSizeCallback(mWindow, reshapeCallback);
}

//---------------------------------------------------------------------------------------
// PERSPECTIVE MOUSE REPORTING CALLBACK SETUP
void Display::perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
	Controls::getInstance()->perspectiveMouseMovement(xpos, ypos);
}
extern "C" void perspectiveMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	Display::getInstance()->perspectiveMouseHandler(window, (float)xpos, (float)ypos);
}
void Display::setCurorPosToPerspectiveCalc() noexcept
{
	Controls::getInstance()->mRenewFPP = true;
	::glfwSetCursorPosCallback(mWindow, perspectiveMouseCallback);
	mMouseReporting = MouseReporting::PERSPECTIVE;
}

//---------------------------------------------------------------------------------------
// STANDARD MOUSE REPORTING CALLBACK SETUP
void Display::standardMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
	Controls::getInstance()->standardMouseMovement(xpos, ypos);
}
extern "C" void standardMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	Display::getInstance()->standardMouseHandler(window, (float)xpos, (float)ypos);
}
void Display::setCurorPosToStandardCalc() noexcept
{
	::glfwSetCursorPosCallback(mWindow, standardMouseCallback);
	mMouseReporting = MouseReporting::STANDARD;
}

//---------------------------------------------------------------------------------------
// MOUSE SCROLL REPROTING CALLBACK SETUP
void Display::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
	Controls::getInstance()->mouseScrollWheelMovement(xpos, ypos);
}
extern "C" void scrollCallback(GLFWwindow * window, double xpos, double ypos)
{
	Display::getInstance()->scrollHandler(window, (float)xpos, (float)ypos);
}
void Display::setScrollWheelHandler() noexcept
{
	::glfwSetScrollCallback(mWindow, scrollCallback);
}
}  // end namespace AA