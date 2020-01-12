#include "AADisplay.h"
#include <glm/glm.hpp>
#include "AAViewport.h"
#include <iostream>

#define VIEWPORT AAViewport::getInstance()

// --------------
// Public Methods
// --------------

AADisplay* AADisplay::getInstance()
{
  static AADisplay* display = new AADisplay();
  return display;
}

AADisplay::~AADisplay()
{
  glfwTerminate();
}

void AADisplay::setWindowClearColor(glm::vec3 rgb)
{
  if (rgb.x < 0.f || rgb.x > 1.0f || rgb.y < 0.f || rgb.y > 1.0f || rgb.z < 0.f || rgb.z > 1.0f)
  {
    //std::cout << "WARNING: Out of range value on setWindowClearColor, values should be between 0.f and 1.f\n";
  }
  mWindowClearColor = rgb;
}

int AADisplay::getWindowWidth()
{
  return mWindowWidth;
}

int AADisplay::getWindowHeight()
{
  return mWindowHeight;
}

bool AADisplay::getIsWindowFullScreen()
{
  return mWindowIsFullScreen;
}

GLFWwindow* AADisplay::getWindow()
{
  return mWindow;
}

void AADisplay::setCursorToVisible()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  mCursorMode = CursorMode::VISIBLE;
}

void AADisplay::setCursorToHidden()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  mCursorMode = CursorMode::HIDDEN;
}

void AADisplay::setCursorToDisabled()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  mCursorMode = CursorMode::FPS;
}

void AADisplay::setWindowTitle(const char* name)
{
  glfwSetWindowTitle(mWindow, name);
}

void AADisplay::setWindowSize(int width, int height, int xpos, int ypos)
{
  glfwSetWindowMonitor(mWindow, nullptr, xpos, ypos, width, height, 0);
  mWindowIsFullScreen = false;
}

void AADisplay::setWindowSize(int width, int height, bool center)
{

  // turn off fullscreen to get frame sizes
  glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  // get work area sizes after turning off full screen
  int x, y, w, h;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

  // get frame sizes after turning off full screen
  int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
  glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

  int xPos = (w / 2) - (width / 2) + ((frameSizeLeft + frameSizeRight) / 2);
  int yPos = (h / 2) - (height / 2) + ((frameSizeTop + frameSizeBot) / 2);

  glfwSetWindowMonitor(mWindow, nullptr, xPos, yPos, width, height, 0);
  mWindowIsFullScreen = false;
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
    setFullscreenToOn();
  }
}

void AADisplay::setFullscreenToOn()
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

void AADisplay::setFullscreenToOff()
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

  // set to a better size in windowed mode
  glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  mWindowIsFullScreen = false;
}

void AADisplay::clearBackBuffer() const
{
  glClearColor(mWindowClearColor.x, mWindowClearColor.y, mWindowClearColor.z, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AADisplay::swapWindowBuffers() const
{
  glfwSwapBuffers(mWindow);
}

void AADisplay::keepWindowOpen()
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

void AADisplay::closeWindow()
{
  glfwSetWindowShouldClose(mWindow, true);
}

void AADisplay::initGLFW()
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
    std::cout << "failed to init glad\n";
    char tmp;
    std::cin >> tmp;
    exit(-1);
  }

  AAViewport::getInstance()->resetViewportVars();

}

void AADisplay::resetStateDataToDefault()
{
  mCursorMode = CursorMode::VISIBLE;
  mWindowIsFullScreen = false;
  mWindowWidth = 800;
  mWindowHeight = 600;
  mXPos = 0;
  mYPos = 0;
  mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);
  glfwDestroyWindow(mWindow);
  VIEWPORT->resetViewportVars();
}

// ---------------
// GLFW3 callbacks
// ---------------

void AADisplay::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
  //std::cout << "reshape window called: " << width << ", " << height << '\n';

  mWindowWidth = width;
  mWindowHeight = height;
  glViewport(0, 0, mWindowWidth, mWindowHeight);
  AAViewport::getInstance()->windowSizeChanged();
  //AAViewport::getInstance()->setToPerspective();  // hack  todo

  //std::cout << "reshape window finished: " << mWindowWidth << ", " << mWindowHeight << '\n';

}
extern "C" void reshapeCallback(GLFWwindow * window, int w, int h)
{
  AADisplay::getInstance()->reshapeWindowHandler(window, w, h);
}
void AADisplay::setReshapeWindowHandler()
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
void AADisplay::setCurorPosToPerspectiveCalc()
{
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
void AADisplay::setCurorPosToStandardCalc()
{
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
void AADisplay::setScrollWheelHandler()
{
  ::glfwSetScrollCallback(mWindow, ::scrollCallback);
}
