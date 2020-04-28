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
#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace AA
{

enum class MouseReporting { STANDARD, PERSPECTIVE };

class Display
{
public:

  static Display* getInstance();

  ~Display();

  int getWindowWidth() noexcept;
  int getWindowHeight() noexcept;
  bool getIsWindowFullScreen() noexcept;
  GLFWwindow* getWindow() noexcept;
  int getCursorMode() const noexcept;
  MouseReporting getMouseReportingMode() const noexcept;

  void setCursorToVisible() noexcept;   // to use the os pointer
  void setCursorToHidden() noexcept;    // for hidden but still there (render your own pointer)
  void setCursorToDisabled() noexcept;  // for first person hidden mouse type

  void setWindowClearColor(glm::vec3 rgb) noexcept;
  void setWindowSize(int width, int height, int xpos, int ypos) noexcept;
  void setWindowSize(int width, int height, bool center = true) noexcept;
  void setWindowSize(const char to) noexcept;
  void setWindowTitle(const char* name) noexcept;

  void closeWindow() noexcept;

  // PUBLIC HANDLERS FOR GLFW
  void reshapeWindowHandler(GLFWwindow* window, int width, int height);
  void perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos);
  void standardMouseHandler(GLFWwindow* window, float xpos, float ypos);
  void scrollHandler(GLFWwindow* window, float xpos, float ypos);

  friend class Loop;

private:

  // internal functions 
  void toggleFullScreen() noexcept;
  void setWindowToFullscreen() noexcept;
  void setFullscreenToOff() noexcept;

  void setWindowToMaximized() noexcept;
  void setWindowToFullscreenBorderless() noexcept;

  void keepWindowOpen() noexcept;

  void clearBackBuffer() const noexcept;
  void swapWindowBuffers() const noexcept;

  void setReshapeWindowHandler() noexcept;
  void setCurorPosToPerspectiveCalc() noexcept;
  void setCurorPosToStandardCalc() noexcept;
  void setScrollWheelHandler() noexcept;

  void initGLFW() noexcept;
  void initFromEngine();
  void resetStateDataToDefault();

  // state data
  MouseReporting mMouseReporting = MouseReporting::STANDARD;
  bool mWindowIsFullScreen = false;
  int mWindowWidth = 800, mWindowHeight = 600;
  int mXPos = 0, mYPos = 0;
  glm::vec3 mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);
  GLFWwindow* mWindow = nullptr;

  // hold for engine to change back to false  so it can know if it should resize its viewport
  bool mWindowSizeChanged = true;

};

}  // end namespace AA