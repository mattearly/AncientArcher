#include <glad/glad.h>
#include "AADisplay.h"
#include "AAViewport.h"
#include "AAControls.h"
#include <memory>
#include <iostream>

AADisplay* AADisplay::getInstance()
{
  static std::unique_ptr<AADisplay> display = std::make_unique<AADisplay>();
  return display.get();
}

AADisplay::AADisplay()
{

}

AADisplay::~AADisplay()
{
  glfwTerminate();
}

void AADisplay::setWindowClearColor(glm::vec3 rgb) noexcept
{
  if (rgb.x < 0.f || rgb.x > 1.0f || rgb.y < 0.f || rgb.y > 1.0f || rgb.z < 0.f || rgb.z > 1.0f)
  {
    //std::cout << "WARNING: Out of range value on setWindowClearColor, values should be between 0.f and 1.f\n";
  }
  mWindowClearColor = rgb;
}

int AADisplay::getWindowWidth() noexcept
{
  return mWindowWidth;
}

int AADisplay::getWindowHeight() noexcept
{
  return mWindowHeight;
}

bool AADisplay::getIsWindowFullScreen() noexcept
{
  return mWindowIsFullScreen;
}

GLFWwindow* AADisplay::getWindow() noexcept
{
  return mWindow;
}

void AADisplay::setCursorToVisible() noexcept
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  mCursorMode = CursorMode::VISIBLE;
}

void AADisplay::setCursorToHidden() noexcept
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  mCursorMode = CursorMode::HIDDEN;

}

void AADisplay::setCursorToDisabled() noexcept
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  mCursorMode = CursorMode::FPS;
}

void AADisplay::setWindowTitle(const char* name) noexcept
{
  glfwSetWindowTitle(mWindow, name);
}

void AADisplay::setWindowSize(int width, int height, int xpos, int ypos) noexcept
{
  glfwSetWindowMonitor(mWindow, nullptr, xpos, ypos, width, height, 0);
  mWindowIsFullScreen = false;
}

void AADisplay::setWindowSize(int width, int height, bool center) noexcept
{

  // turn off fullscreen to get frame sizes
  glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  // get work area sizes after turning off full screen
  int x, y, w, h;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

  // get frame sizes after turning off full screen
  int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
  glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

  const int xPos = (w / 2) - (width / 2) + ((frameSizeLeft + frameSizeRight) / 2);
  const int yPos = (h / 2) - (height / 2) + ((frameSizeTop + frameSizeBot) / 2);

  glfwSetWindowMonitor(mWindow, nullptr, xPos, yPos, width, height, 0);
  mWindowIsFullScreen = false;
}

void AADisplay::setWindowSize(const char to)
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



// -----------------
// Private Functions
// -----------------

void AADisplay::toggleFullScreen()
{
  if (mWindowIsFullScreen)
  {
    setFullscreenToOff();
  }
  else
  {
    setWindowToFullscreen();
  }
}

void AADisplay::setWindowToFullscreen() noexcept
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

void AADisplay::setFullscreenToOff() noexcept
{
  // turn off fullscreen to get frame sizes
  glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  // get work area sizes after turning off full screen
  int x, y, w, h;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

  // get frame sizes after turning off full screen
  int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
  glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

  // update window size and positions
  mXPos = x + frameSizeLeft;
  mYPos = y + frameSizeTop;
  mWindowWidth = w - frameSizeLeft - frameSizeRight;
  mWindowHeight = h - frameSizeTop - frameSizeBot;

  // set size in windowed mode
  glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  mWindowIsFullScreen = false;
}

void AADisplay::setWindowToMaximized() noexcept
{
  // turn off fullscreen so the maximize works (glfw specification)
  if (mWindowIsFullScreen) {
    glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);
    mWindowIsFullScreen = false;
  }
  glfwMaximizeWindow(mWindow);
}

void AADisplay::setWindowToFullscreenBorderless()
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

void AADisplay::clearBackBuffer() const noexcept
{
  glClearColor(mWindowClearColor.x, mWindowClearColor.y, mWindowClearColor.z, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AADisplay::swapWindowBuffers() const noexcept
{
  glfwSwapBuffers(mWindow);
}

void AADisplay::keepWindowOpen() noexcept
{
  glfwSetWindowShouldClose(mWindow, false);
}

void AADisplay::toggleCursor()
{
  switch (mCursorMode)
  {
  case CursorMode::FPS:
    this->setCursorToVisible();
    break;
  case CursorMode::VISIBLE:
    this->setCursorToDisabled();
    break;
  default:
    break;
  }
}

void AADisplay::closeWindow() noexcept
{
  glfwSetWindowShouldClose(mWindow, true);
}

void AADisplay::initGLFW() noexcept
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void AADisplay::initFromEngine()
{
  initGLFW();

  mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "AncientArcher", nullptr, nullptr);
  if (!mWindow)
  {
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(mWindow);

  setReshapeWindowHandler();

  //setCurorPosToPerspectiveCalc();
  setCurorPosToStandardCalc();

  setScrollWheelHandler();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context)
  {
    //todo: logging without iostream
    //std::cout << "failed to init glad\n";
    //char tmp;
    //std::cin >> tmp;
    exit(-1);
  }

  AAViewport::getInstance()->resetViewportVars();

}

void AADisplay::resetStateDataToDefault()
{
  setCursorToVisible();
  mWindowIsFullScreen = false;
  mWindowWidth = 800;
  mWindowHeight = 600;
  mXPos = 0;
  mYPos = 0;
  mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);
  mMouseReporting = MouseReporting::STANDARD;
  glfwDestroyWindow(mWindow);
  AAViewport::getInstance()->resetViewportVars();
}

// ---------------
// GLFW3 callbacks
// ---------------

void AADisplay::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
  mWindowWidth = width;
  mWindowHeight = height;
  glViewport(0, 0, mWindowWidth, mWindowHeight);
  AAViewport::getInstance()->windowViewportChanged();
}

extern "C" void reshapeCallback(GLFWwindow * window, int w, int h)
{
  AADisplay::getInstance()->reshapeWindowHandler(window, w, h);
}

void AADisplay::setReshapeWindowHandler() noexcept
{
  ::glfwSetFramebufferSizeCallback(mWindow, ::reshapeCallback);
}

void AADisplay::perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->perspectiveMouseMovement(xpos, ypos);
}
extern "C" void perspectiveMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
  AADisplay::getInstance()->perspectiveMouseHandler(window, (float)xpos, (float)ypos);
}
void AADisplay::setCurorPosToPerspectiveCalc() noexcept
{
  mMouseReporting = MouseReporting::PERSPECTIVE;
  ::glfwSetCursorPosCallback(mWindow, ::perspectiveMouseCallback);
}

void AADisplay::standardMouseHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->standardMouseMovement(xpos, ypos);
}
extern "C" void standardMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
  AADisplay::getInstance()->standardMouseHandler(window, (float)xpos, (float)ypos);
}
void AADisplay::setCurorPosToStandardCalc() noexcept
{
  mMouseReporting = MouseReporting::STANDARD;
  ::glfwSetCursorPosCallback(mWindow, ::standardMouseCallback);
}

void AADisplay::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseScrollWheelMovement(xpos, ypos);
}
extern "C" void scrollCallback(GLFWwindow * window, double xpos, double ypos)
{
  AADisplay::getInstance()->scrollHandler(window, (float)xpos, (float)ypos);
}
void AADisplay::setScrollWheelHandler() noexcept
{
  ::glfwSetScrollCallback(mWindow, ::scrollCallback);
}
