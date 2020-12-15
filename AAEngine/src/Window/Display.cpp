/*
Display
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
#include "../../include/Window/Display.h"
#include "../../include/Controls/Controls.h"
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace AA
{
//Display* Display::Get()
//{
//	static Display* display = new Display();
//	return display;
//}

Display::~Display()  // breaks rule of 5
{
	glfwTerminate();
}

void Display::SetWindowClearColor(glm::vec3 rgb) noexcept
{
	if (rgb.x < 0.f || rgb.x > 1.0f || rgb.y < 0.f || rgb.y > 1.0f || rgb.z < 0.f || rgb.z > 1.0f)
	{
		//std::cout << "WARNING: Out of range value on SetWindowClearColor, values should be between 0.f and 1.f\n";
	}
	mWindowClearColor = rgb;
}

int Display::GetWindowWidth() noexcept
{
	return mWindowWidth;
}

int Display::GetWindowHeight() noexcept
{
	return mWindowHeight;
}

bool Display::GetIsWindowFullScreen() noexcept
{
	return mWindowIsFullScreen;
}

GLFWwindow* Display::GetWindow() noexcept
{
	return mWindow;
}

int Display::GetCursorMode() const noexcept
{
	return glfwGetInputMode(mWindow, GLFW_CURSOR);
}

MouseReporting Display::GetMouseReportingMode() const noexcept
{
	return mMouseReporting;
}

void Display::SetCursorToVisible() noexcept
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Display::SetCursorToHidden() noexcept
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Display::SetCursorToDisabled() noexcept
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Display::SetWindowTitle(const char* name) noexcept
{
	glfwSetWindowTitle(mWindow, name);
}

void Display::SetWindowSize(int width, int height, int xpos, int ypos) noexcept
{
	if (width > 0 && height > 0)
	{
		glfwSetWindowMonitor(mWindow, nullptr, xpos, ypos, width, height, 0);
	}
	else
	{
		glfwSetWindowMonitor(mWindow, nullptr, xpos, ypos, mWindowWidth, mWindowHeight, 0);

	}
	mWindowIsFullScreen = false;
}

void Display::SetWindowSize(int width, int height, bool center) noexcept
{
	// turn off fullscreen to Get frame sizes
	if (mWindowIsFullScreen)
		glfwSetWindowMonitor(mWindow, nullptr, 0, 0, mWindowWidth, mWindowHeight, 0);

	// Get work area sizes after turning off full screen
	int x, y, w, h;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

	// Get frame sizes after turning off full screen
	int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
	glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

	const int xPos = (w / 2) - (width / 2) + ((frameSizeLeft + frameSizeRight) / 2);
	const int yPos = (h / 2) - (height / 2) + ((frameSizeTop + frameSizeBot) / 2);

	mXPos = xPos;
	mYPos = yPos;
	glfwSetWindowMonitor(mWindow, nullptr, xPos, yPos, width, height, 0);
	mWindowIsFullScreen = false;
}

void Display::SetWindowSize(const char to) noexcept
{
	switch (to)
	{
		// m = maximize
	case 'm':
		SetWindowToMaximized();
		break;

		// f = fullscreen
	case 'f':
		SetWindowToFullscreen();
		break;

		// b = borderless fullscreen
	case 'b':
		SetWindowToFullscreenBorderless();
		break;

		// undefined = do nothing
	default:
		break;
	}
}

void Display::toggleFullScreen() noexcept
{
	if (mWindowIsFullScreen)
	{
		SetFullscreenToOff();
	}
	else
	{
		SetWindowToFullscreen();
	}
}

void Display::SetWindowToFullscreen() noexcept
{
	int x, y, w, h;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);
	int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
	glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

	mXPos = 0;
	mYPos = 0;
	mWindowWidth = w + frameSizeLeft + frameSizeRight;
	mWindowHeight = h + frameSizeTop + frameSizeBot;

	glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

	mWindowIsFullScreen = true;
}

void Display::SetFullscreenToOff() noexcept
{
	// turn off fullscreen to Get frame sizes
	glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

	// Get work area sizes after turning off full screen
	int x, y, w, h;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

	// Get frame sizes after turning off full screen
	int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
	glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

	// update window size and positions
	mXPos = x + frameSizeLeft;
	mYPos = y + frameSizeTop;
	mWindowWidth = w - frameSizeLeft - frameSizeRight;
	mWindowHeight = h - frameSizeTop - frameSizeBot;

	// Set size in windowed mode
	glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

	mWindowIsFullScreen = false;
}

void Display::SetWindowToMaximized() noexcept
{
	// turn off fullscreen so the maximize works (glfw specification)
	if (mWindowIsFullScreen) {
		glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);
		mWindowIsFullScreen = false;
	}
	glfwMaximizeWindow(mWindow);
}

void Display::SetWindowToFullscreenBorderless() noexcept
{
	if (mWindowIsFullScreen)
	{
		toggleFullScreen();
	}

	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	mXPos = mYPos = 0;
	mWindowHeight = mode->height;
	mWindowWidth = mode->width;

	glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, mode->refreshRate);
	mWindowIsFullScreen = true;
}

void Display::clearBackBuffer() const noexcept
{
	glClearColor(mWindowClearColor.x, mWindowClearColor.y, mWindowClearColor.z, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::swapWindowBuffers() const noexcept
{
	glfwSwapBuffers(mWindow);
}

void Display::keepWindowOpen() noexcept
{
	glfwSetWindowShouldClose(mWindow, 0);
}

void Display::closeWindow() noexcept
{
	glfwSetWindowShouldClose(mWindow, 1);
}

void Display::initGLFW() noexcept
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// with core profile, you have to create and manage your own VAO's, no default given
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void Display::initFromEngine()
{
	initGLFW();

	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "AncientArcher", nullptr, nullptr);
	if (!mWindow)
	{
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(mWindow);

	//SetReshapeWindowHandler();

	////SetCurorPosToPerspectiveCalc();
	//SetCurorPosToStandardCalc();

	//SetScrollWheelHandler();

	//SetResizeWindowHandler();

	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context) -- moved to engine
	//{
	//  //todo: logging without iostream
	//  //std::cout << "failed to init glad\n";
	//  //char tmp;
	//  //std::cin >> tmp;
	//  exit(-1);
	//}
}

void Display::ResetStateDataToDefault()
{
	mWindowIsFullScreen = false;
	mWindowWidth = 800, mWindowHeight = 600;
	mXPos = mYPos = 0;
	mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);

	glfwDestroyWindow(mWindow);

	mWindow = nullptr;
}
}  // end namespace AA