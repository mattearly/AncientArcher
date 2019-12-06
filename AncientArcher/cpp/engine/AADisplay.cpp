#include "AADisplay.h"
#include <glm/glm.hpp>
#include "AAViewport.h"
#include <iostream>

// --------------
// Public Methods
// --------------

AADisplay* AADisplay::getInstance()
{
  static AADisplay* display = new AADisplay();
  return display;
}

AADisplay::AADisplay()
{
  initGLFW();

  mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "AncientArcher", nullptr, nullptr);
  if (!mWindow)
  {
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(mWindow);

  initReshapeWindowHandler();

  initMouseHandler();

  initMouseScrollHandler();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context)
  {
    std::cout << "failed to init glad\n";
    char tmp;
    std::cin >> tmp;
    exit(-1);
  }

}

AADisplay::~AADisplay()
{
  glfwTerminate();
}

void AADisplay::setCursorToVisible()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void AADisplay::setCursorToHidden()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void AADisplay::setCursorToDisabled()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

void AADisplay::setWindowClearColor(glm::vec3 rgb)
{
  if (rgb.x < 0.f || rgb.x > 1.0f || rgb.y < 0.f || rgb.y > 1.0f || rgb.z < 0.f || rgb.z > 1.0f)
  {
    //std::cout << "WARNING: Out of range value on setWindowClearColor, values should be between 0.f and 1.f\n";
  }
  mWindowClearColor = rgb;
}

int AADisplay::getScreenWidth()
{
  return mWindowWidth;
}

int AADisplay::getScreenHeight()
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
  mWindowWidth = w + x + frameSizeLeft + frameSizeRight;
  mWindowHeight = h + y + frameSizeTop + frameSizeBot;

  glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  mWindowIsFullScreen = true;
}

void AADisplay::setFullscreenToOff()
{
  int x, y, w, h;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

  int frameSizeLeft, frameSizeTop, frameSizeRight, frameSizeBot;
  glfwGetWindowFrameSize(mWindow, &frameSizeLeft, &frameSizeTop, &frameSizeRight, &frameSizeBot);

  mXPos = x;
  mYPos = y;
  mWindowWidth = w;
  mWindowHeight = h;

  glfwSetWindowMonitor(mWindow, nullptr, mXPos, mYPos, mWindowWidth, mWindowHeight, 0);

  glfwMaximizeWindow(mWindow);

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

// ---------------
// GLFW3 callbacks
// ---------------

void AADisplay::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
  std::cout << "reshape window called: " << width << ", " << height << '\n';

  mWindowWidth = width;
  mWindowHeight = height;
  glViewport(0, 0, mWindowWidth, mWindowHeight);
  AAViewport::getInstance()->setToPerspective();  // hack

  std::cout << "reshape window finished: " << mWindowWidth << ", " << mWindowHeight << '\n';

}

void AADisplay::mouseHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseMovement(xpos, ypos);
}

void AADisplay::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseScrollWheelMovement(xpos, ypos);
}

extern "C" void reshapeCallback(GLFWwindow * window, int w, int h)
{
  AADisplay::getInstance()->reshapeWindowHandler(window, w, h);
}

extern "C" void mouseCallback(GLFWwindow * window, double xpos, double ypos)
{
  AADisplay::getInstance()->mouseHandler(window, (float)xpos, (float)ypos);
}

extern "C" void scrollCallback(GLFWwindow * window, double xpos, double ypos)
{
  AADisplay::getInstance()->scrollHandler(window, (float)xpos, (float)ypos);
}

void AADisplay::initReshapeWindowHandler()
{
  ::glfwSetFramebufferSizeCallback(mWindow, ::reshapeCallback);
}

void AADisplay::initMouseHandler()
{
  ::glfwSetCursorPosCallback(mWindow, ::mouseCallback);
}

void AADisplay::initMouseScrollHandler()
{
  ::glfwSetScrollCallback(mWindow, ::scrollCallback);
}
