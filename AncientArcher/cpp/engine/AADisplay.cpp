#include "AADisplay.h"
#include <iostream>
#include <glm/glm.hpp>
#include "AAViewport.h"

AADisplay* AADisplay::getInstance()
{
  static AADisplay* display = new AADisplay();
  return display;
}

void AADisplay::keepWindowOpen()
{
  glfwSetWindowShouldClose(mWindow, false);
}

void AADisplay::closeWindow()
{
  glfwSetWindowShouldClose(mWindow, true);
}

AADisplay::AADisplay()
{
  initGLFW();

  int mLaunchedMainScreenWindowWidth, mLaunchedMainScreenWindowHeight;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &mXPos, &mYPos, &mLaunchedMainScreenWindowWidth, &mLaunchedMainScreenWindowHeight);

  mWindow = glfwCreateWindow(
    mLaunchedMainScreenWindowWidth,
    mLaunchedMainScreenWindowHeight,
    "AncientArcher",
    nullptr,
    nullptr
  );
  if (!mWindow)
  {
    std::cout << "Failed to init GLFW3 window\n";
    glfwTerminate();
  }

  int mWindowFrameSizeLeft, mWindowFrameSizeTop, mWindowFrameSizeRight, mWindowFrameSizeBottom;
  glfwGetWindowFrameSize(mWindow, &mWindowFrameSizeLeft, &mWindowFrameSizeTop, &mWindowFrameSizeRight, &mWindowFrameSizeBottom);

  glfwMakeContextCurrent(mWindow);

  initReshapeWindowHandler();

  initMouseHandler();

  initMouseScrollHandler();


  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context)
  {
    std::cout << "failed to init glad\n";
    exit(-1);
  }

  setFullscreenToOff();

}

AADisplay::~AADisplay()
{
  glfwTerminate();
}

void AADisplay::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
  mWindowWidth = width;
  mWindowHeight = height;
  glViewport(mXPos, mYPos, mWindowWidth, mWindowHeight);
  std::cout << "reshape Window Handler called.\n";
}

void AADisplay::mouseHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseMovement(xpos, ypos);
}

void AADisplay::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseScrollWheelMovement(xpos, ypos);
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
  mWindowWidth = width;
  mWindowHeight = height;
  mXPos = xpos; // (xpos == 0) ? mWindowFrameSizeLeft - mWindowFrameSizeRight : xpos;
  mYPos = ypos; // (ypos == 0) ? mWindowFrameSizeTop - mWindowFrameSizeBottom : ypos;
  glfwSetWindowMonitor(
    mWindow,
    nullptr,
    mXPos,
    mYPos,
    mWindowWidth,
    mWindowHeight,
    0
  );
  mWindowIsFullScreen = false;
}

void AADisplay::setWindowClearColor(glm::vec3 rgb)
{
  if (rgb.x < 0.f || rgb.x > 1.0f || rgb.y < 0.f || rgb.y > 1.0f || rgb.z < 0.f || rgb.z > 1.0f)
  {
    std::cout << "WARNING: Out of range value on setWindowClearColor, values should be between 0.f and 1.f\n";
  }
  mWindowClearColor = rgb;
}

void AADisplay::setFullscreenToOn()
{
  int x, y, w, h;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);

  mXPos = x;
  mYPos = y;
  mWindowWidth = w;
  mWindowHeight = h;

  glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), 0, 0, mWindowWidth, mWindowHeight, 0);
  
  glViewport(mXPos, mYPos, mWindowWidth, mWindowHeight);
  
  mWindowIsFullScreen = true;
}

void AADisplay::setFullscreenToOff()
{
  //mXPos = 0; // mWindowFrameSizeLeft - mWindowFrameSizeRight;
  //mYPos = 0; // mWindowFrameSizeTop - mWindowFrameSizeBottom;
  //mWindowWidth = mLaunchedMainScreenWindowWidth;
  //mWindowHeight = mLaunchedMainScreenWindowHeight;
  glfwSetWindowMonitor(
    mWindow,
    nullptr,
    mXPos,
    mYPos,
    mWindowWidth,
    mWindowHeight,
    0
  );
  glViewport(mXPos, mYPos, mWindowWidth, mWindowHeight);
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

