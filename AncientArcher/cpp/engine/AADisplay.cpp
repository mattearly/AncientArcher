#include "AADisplay.h"
#include <iostream>

AADisplay* AADisplay::getInstance()
{
  static AADisplay* display = new AADisplay();
  return display;
}

AADisplay::AADisplay()
{
  initGLFW(); 

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

  glfwGetWindowFrameSize(mWindow, &mWindowFrameSizeLeft, &mWindowFrameSizeTop, &mWindowFrameSizeRight, &mWindowFrameSizeBottom);

  glfwMakeContextCurrent(mWindow);
  initReshapeWindowHandler();
  initMouseHandler();
  initMouseScrollHandler();
  enableCursor();
  setFullscreenToOff();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context)
  {
    std::cout << "failed to init glad\n";
    exit(-1);
  }
}

AADisplay::~AADisplay()
{
  glfwTerminate();
}

void AADisplay::reshapeWindowHandler(GLFWwindow* window, int width, int height)
{
  glViewport(mXPos, mYPos, width, height);
  mWindowWidth = width;
  mWindowHeight = height;
  gWindowRatioChanged = true;
}

void AADisplay::mouseHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseMovement(xpos, ypos);
}

void AADisplay::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
  AAControls::getInstance()->mouseScrollWheelMovement(xpos, ypos);
}

void AADisplay::enableCursor()
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void AADisplay::disableCursor()
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
  mXPos = (xpos == 0) ? mWindowFrameSizeLeft - mWindowFrameSizeRight : xpos;
  mYPos = (ypos == 0) ? mWindowFrameSizeTop - mWindowFrameSizeBottom : ypos;
  glfwSetWindowMonitor(
    mWindow, 
    nullptr, 
    mXPos,
    mYPos,
    mWindowWidth, 
    mWindowHeight, 
    0
  );
  gWindowRatioChanged = true;
  mWindowIsFullScreen = false;
}

void AADisplay::setWindowClearColor(glm::vec3 rgb)
{
  mWindowClearColor = rgb;
}

void AADisplay::setFullscreenToOn()
{
  mWindowWidth = 1920;
  mWindowHeight = 1080;
  glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), 0, 0, mWindowWidth, mWindowHeight, 0);
  gWindowRatioChanged = true;
  mWindowIsFullScreen = true;
}

void AADisplay::setFullscreenToOff()
{
  mXPos = mWindowFrameSizeLeft - mWindowFrameSizeRight;
  mYPos = mWindowFrameSizeTop - mWindowFrameSizeBottom;
  glfwSetWindowMonitor(
    mWindow,
    nullptr,
    mXPos,
    mYPos,

    mLaunchedMainScreenWindowWidth,
    mLaunchedMainScreenWindowHeight,
    0
  );
  gWindowRatioChanged = true;
  mWindowIsFullScreen = false;
}

void AADisplay::clearBackBuffer() const
{
  glClearColor(mWindowClearColor.x, mWindowClearColor.y, mWindowClearColor.z, 0.0f);  // red
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

