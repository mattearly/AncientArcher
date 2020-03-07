// ---------------
// GLFW3 callbacks
// ---------------
#include "AADisplay.h"
#include "AAControls.h"

//---------------------------------------------------------------------------------------
// RESHAPE WINDOW CALLBACK SETUP
void AADisplay::reshapeWindowHandler(GLFWwindow* window, int width, int height)
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
  AADisplay::getInstance()->reshapeWindowHandler(window, w, h);
}
void AADisplay::setReshapeWindowHandler() noexcept
{
  ::glfwSetFramebufferSizeCallback(mWindow, ::reshapeCallback);
}

//---------------------------------------------------------------------------------------
// PERSPECTIVE MOUSE REPORTING CALLBACK SETUP
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
  ::glfwSetCursorPosCallback(mWindow, ::perspectiveMouseCallback);
  mMouseReporting = MouseReporting::PERSPECTIVE;
}

//---------------------------------------------------------------------------------------
// STANDARD MOUSE REPORTING CALLBACK SETUP
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
  ::glfwSetCursorPosCallback(mWindow, ::standardMouseCallback);
  mMouseReporting = MouseReporting::STANDARD;
}

//---------------------------------------------------------------------------------------
// MOUSE SCROLL REPROTING CALLBACK SETUP
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
