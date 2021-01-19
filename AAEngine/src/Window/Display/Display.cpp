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
#include "../../../include/Window/Display/Display.h"
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace AA
{
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
	int width, height;

	glfwGetWindowSize(mWindow, &width, &height);

	return width;
}

int Display::GetWindowHeight() noexcept
{
	int width, height;

	glfwGetWindowSize(mWindow, &width, &height);

	return height;
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

/// <summary>
/// Sets the window from a list of presets m = max, f = fullscreen, b = borderless
/// </summary>
/// <param name="to">'m' = maximize, 'f' = fullscreen, 'b' = fullscreen borderless. All other specifications are ignored with undefined behavior.</param>
/// <returns></returns>
void Display::SetWindowSize(const char to) noexcept
{
	switch (to)
	{
		// m = maximize
	case 'm':
		setWindowToMaximized();
		break;

		// f = fullscreen
	case 'f':
		setWindowToFullscreen();
		break;

		// b = borderless fullscreen
	case 'b':
		setWindowToFullscreenBorderless();
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
		setWindowToFullscreen();
	}
}

void Display::setWindowToFullscreen() noexcept
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

void Display::setWindowToMaximized() noexcept
{
	// turn off fullscreen so the maximize works (glfw specification)
	if (mWindowIsFullScreen) {
		SetFullscreenToOff();
	}
	glfwMaximizeWindow(mWindow);
}

void Display::setWindowToFullscreenBorderless() noexcept
{
	if (mWindowIsFullScreen)
	{
		SetFullscreenToOff();
	}

	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	mXPos = mYPos = 0;
	mWindowHeight = mode->height;
	mWindowWidth = mode->width;

	glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, mode->refreshRate);

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

void Display::initDisplayFromEngine()
{
	initGLFW();

	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "AncientArcher", nullptr, nullptr);
	if (!mWindow)
	{
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(mWindow);

	//setReshapeWindowHandler();

	////SetCurorReadToFPPCalc();
	//SetCurorReadToStandardCalc();

	//setScrollWheelHandler();

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

void Display::resetStateDataToDefault()
{
	if (mWindowIsFullScreen)
		toggleFullScreen();
	//mWindowIsFullScreen = false;
	mWindowWidth = 800, mWindowHeight = 600;
	mXPos = mYPos = 0;
	mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);

	glfwDestroyWindow(mWindow);

	mWindow = nullptr;
}


/////// PULL CONTROLS BUTTON STATE FROM DISPLAY ////////
void Display::pullButtonStateEvents()
{
	glfwPollEvents();

	// esc
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mButtonState.esc = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		mButtonState.esc = false;
	}
	// function keys
	if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_PRESS)
	{
		mButtonState.f1 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_RELEASE)
	{
		mButtonState.f1 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_PRESS)
	{
		mButtonState.f2 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_RELEASE)
	{
		mButtonState.f2 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_PRESS)
	{
		mButtonState.f3 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_RELEASE)
	{
		mButtonState.f3 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_PRESS)
	{
		mButtonState.f4 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_RELEASE)
	{
		mButtonState.f4 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_PRESS)
	{
		mButtonState.f5 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_RELEASE)
	{
		mButtonState.f5 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_PRESS)
	{
		mButtonState.f6 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_RELEASE)
	{
		mButtonState.f6 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_PRESS)
	{
		mButtonState.f7 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_RELEASE)
	{
		mButtonState.f7 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_PRESS)
	{
		mButtonState.f8 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_RELEASE)
	{
		mButtonState.f8 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_PRESS)
	{
		mButtonState.f9 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_RELEASE)
	{
		mButtonState.f9 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_PRESS)
	{
		mButtonState.f10 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_RELEASE)
	{
		mButtonState.f10 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F11) == GLFW_PRESS)
	{
		mButtonState.f11 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F11) == GLFW_RELEASE)
	{
		mButtonState.f11 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F12) == GLFW_PRESS)
	{
		mButtonState.f12 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F12) == GLFW_RELEASE)
	{
		mButtonState.f12 = false;
	}
	// number key row
	if (glfwGetKey(mWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
	{
		mButtonState.graveAccent = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
	{
		mButtonState.graveAccent = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
	{
		mButtonState.n1 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_RELEASE)
	{
		mButtonState.n1 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS)
	{
		mButtonState.n2 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_RELEASE)
	{
		mButtonState.n2 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_PRESS)
	{
		mButtonState.n3 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_RELEASE)
	{
		mButtonState.n3 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_PRESS)
	{
		mButtonState.n4 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_RELEASE)
	{
		mButtonState.n4 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_PRESS)
	{
		mButtonState.n5 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_RELEASE)
	{
		mButtonState.n5 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_6) == GLFW_PRESS)
	{
		mButtonState.n6 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_6) == GLFW_RELEASE)
	{
		mButtonState.n6 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_7) == GLFW_PRESS)
	{
		mButtonState.n7 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_7) == GLFW_RELEASE)
	{
		mButtonState.n7 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_8) == GLFW_PRESS)
	{
		mButtonState.n8 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_8) == GLFW_RELEASE)
	{
		mButtonState.n8 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_9) == GLFW_PRESS)
	{
		mButtonState.n9 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_9) == GLFW_RELEASE)
	{
		mButtonState.n9 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_0) == GLFW_PRESS)
	{
		mButtonState.n0 = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_0) == GLFW_RELEASE)
	{
		mButtonState.n0 = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		mButtonState.minus = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_MINUS) == GLFW_RELEASE)
	{
		mButtonState.minus = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		mButtonState.equal = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_EQUAL) == GLFW_RELEASE)
	{
		mButtonState.equal = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
	{
		mButtonState.backspace = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
	{
		mButtonState.backspace = false;
	}
	// alphabet keys
	if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		mButtonState.a = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_RELEASE)
	{
		mButtonState.a = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_PRESS)
	{
		mButtonState.b = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_RELEASE)
	{
		mButtonState.b = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_PRESS)
	{
		mButtonState.c = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_RELEASE)
	{
		mButtonState.c = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		mButtonState.d = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_RELEASE)
	{
		mButtonState.d = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		mButtonState.e = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_RELEASE)
	{
		mButtonState.e = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
	{
		mButtonState.f = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_RELEASE)
	{
		mButtonState.f = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_G) == GLFW_PRESS)
	{
		mButtonState.g = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_G) == GLFW_RELEASE)
	{
		mButtonState.g = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_H) == GLFW_PRESS)
	{
		mButtonState.h = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_H) == GLFW_RELEASE)
	{
		mButtonState.h = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_I) == GLFW_PRESS)
	{
		mButtonState.i = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_I) == GLFW_RELEASE)
	{
		mButtonState.i = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_J) == GLFW_PRESS)
	{
		mButtonState.j = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_J) == GLFW_RELEASE)
	{
		mButtonState.j = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_K) == GLFW_PRESS)
	{
		mButtonState.k = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_K) == GLFW_RELEASE)
	{
		mButtonState.k = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_L) == GLFW_PRESS)
	{
		mButtonState.l = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_L) == GLFW_RELEASE)
	{
		mButtonState.l = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_PRESS)
	{
		mButtonState.m = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_RELEASE)
	{
		mButtonState.m = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_PRESS)
	{
		mButtonState.n = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_RELEASE)
	{
		mButtonState.n = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_PRESS)
	{
		mButtonState.o = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_RELEASE)
	{
		mButtonState.o = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS)
	{
		mButtonState.p = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_RELEASE)
	{
		mButtonState.p = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mButtonState.q = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_RELEASE)
	{
		mButtonState.q = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_PRESS)
	{
		mButtonState.r = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_RELEASE)
	{
		mButtonState.r = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		mButtonState.s = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_RELEASE)
	{
		mButtonState.s = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_T) == GLFW_PRESS)
	{
		mButtonState.t = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_T) == GLFW_RELEASE)
	{
		mButtonState.t = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_U) == GLFW_PRESS)
	{
		mButtonState.u = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_U) == GLFW_RELEASE)
	{
		mButtonState.u = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_PRESS)
	{
		mButtonState.v = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_RELEASE)
	{
		mButtonState.v = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		mButtonState.w = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_RELEASE)
	{
		mButtonState.w = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
	{
		mButtonState.x = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_RELEASE)
	{
		mButtonState.x = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_PRESS)
	{
		mButtonState.y = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_RELEASE)
	{
		mButtonState.y = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_PRESS)
	{
		mButtonState.z = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_RELEASE)
	{
		mButtonState.z = false;
	}
	// tab-shift-control-alt
	if (glfwGetKey(mWindow, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		mButtonState.tab = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_TAB) == GLFW_RELEASE)
	{
		mButtonState.tab = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		mButtonState.leftShift = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		mButtonState.leftShift = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
	{
		mButtonState.rightShift = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
	{
		mButtonState.rightShift = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		mButtonState.leftControl = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		mButtonState.leftControl = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
	{
		mButtonState.rightControl = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE)
	{
		mButtonState.rightControl = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		mButtonState.leftAlt = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
	{
		mButtonState.leftAlt = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
	{
		mButtonState.rightAlt = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE)
	{
		mButtonState.rightAlt = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		mButtonState.spacebar = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		mButtonState.spacebar = false;
	}
	// brackets
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
	{
		mButtonState.leftSquareBracket = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
	{
		mButtonState.leftSquareBracket = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
	{
		mButtonState.rightSquareBracket = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
	{
		mButtonState.rightSquareBracket = false;
	}
	// slash-quote-semicolon-enter
	if (glfwGetKey(mWindow, GLFW_KEY_BACKSLASH) == GLFW_PRESS)
	{
		mButtonState.backslash = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_BACKSLASH) == GLFW_RELEASE)
	{
		mButtonState.backslash = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
	{
		mButtonState.semiColon = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
	{
		mButtonState.semiColon = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
	{
		mButtonState.apostrophe = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_APOSTROPHE) == GLFW_RELEASE)
	{
		mButtonState.apostrophe = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		mButtonState.enter = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_RELEASE)
	{
		mButtonState.enter = false;
	}
	// comma-period-forwardslash
	if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_PRESS)
	{
		mButtonState.comma = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_RELEASE)
	{
		mButtonState.comma = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PERIOD) == GLFW_PRESS)
	{
		mButtonState.period = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PERIOD) == GLFW_RELEASE)
	{
		mButtonState.period = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SLASH) == GLFW_PRESS)
	{
		mButtonState.forwardSlash = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SLASH) == GLFW_RELEASE)
	{
		mButtonState.forwardSlash = false;
	}
	// printscreen-etc
	if (glfwGetKey(mWindow, GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS)
	{
		mButtonState.printScreen = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PRINT_SCREEN) == GLFW_RELEASE)
	{
		mButtonState.printScreen = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS)
	{
		mButtonState.scrollLock = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_SCROLL_LOCK) == GLFW_RELEASE)
	{
		mButtonState.scrollLock = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PAUSE) == GLFW_PRESS)
	{
		mButtonState.pauseBreak = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PAUSE) == GLFW_RELEASE)
	{
		mButtonState.pauseBreak = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_INSERT) == GLFW_PRESS)
	{
		mButtonState.insert = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_INSERT) == GLFW_RELEASE)
	{
		mButtonState.insert = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_DELETE) == GLFW_PRESS)
	{
		mButtonState.del = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_DELETE) == GLFW_RELEASE)
	{
		mButtonState.del = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_HOME) == GLFW_PRESS)
	{
		mButtonState.home = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_HOME) == GLFW_RELEASE)
	{
		mButtonState.home = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_END) == GLFW_PRESS)
	{
		mButtonState.end = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_END) == GLFW_RELEASE)
	{
		mButtonState.end = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
		mButtonState.pageUp = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PAGE_UP) == GLFW_RELEASE) {
		mButtonState.pageUp = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
		mButtonState.pageDown = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE) {
		mButtonState.pageDown = false;
	}
	// arrows
	if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS) {
		mButtonState.upArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_RELEASE) {
		mButtonState.upArrow = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mButtonState.downArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		mButtonState.downArrow = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
		mButtonState.leftArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_RELEASE) {
		mButtonState.leftArrow = false;
	}
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		mButtonState.rightArrow = true;
	}
	else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
		mButtonState.rightArrow = false;
	}

	// mouse clicks
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		mButtonState.mouseButton1 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		mButtonState.mouseButton1 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		mButtonState.mouseButton2 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		mButtonState.mouseButton2 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
		mButtonState.mouseButton3 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
		mButtonState.mouseButton3 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS) {
		mButtonState.mouseButton4 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_4) == GLFW_RELEASE) {
		mButtonState.mouseButton4 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS) {
		mButtonState.mousebutton5 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_5) == GLFW_RELEASE) {
		mButtonState.mousebutton5 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_6) == GLFW_PRESS) {
		mButtonState.mouseButton6 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_6) == GLFW_RELEASE) {
		mButtonState.mouseButton6 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_7) == GLFW_PRESS) {
		mButtonState.mousebutton7 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_7) == GLFW_RELEASE) {
		mButtonState.mousebutton7 = false;
	}
	if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_8) == GLFW_PRESS) {
		mButtonState.mouseButton8 = true;
	}
	else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_8) == GLFW_RELEASE) {
		mButtonState.mouseButton8 = false;
	}
}

}  // end namespace AA