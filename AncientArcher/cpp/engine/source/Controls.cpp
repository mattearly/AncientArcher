#include <Controls.h>
#include <Display.h>

extern Display g_display;

void Controls::keyboardInput()
{
  // ESC
  if (glfwGetKey(g_display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(g_display.window, true);
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
  }

  // Left Click
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    _keypress->leftClick = true;
  }
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    _keypress->leftClick = false;
  }

  // Right Click
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    _keypress->rightClick = true;
  }
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    _keypress->rightClick = false;
  }

  // Shift
  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    _keypress->leftShift = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    _keypress->leftShift = false;
  }

  // WASD
  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_PRESS) {
    _keypress->w = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_RELEASE) {
    _keypress->w = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_PRESS) {
    _keypress->s = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_RELEASE) {
    _keypress->s = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_PRESS) {
    _keypress->a = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_RELEASE) {
    _keypress->a = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_PRESS) {
    _keypress->d = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_RELEASE) {
    _keypress->d = false;
  }

  // 1, 2, 3
  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_PRESS) {
    _keypress->_1 = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_RELEASE) {
    _keypress->_1 = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_PRESS) {
    _keypress->_2 = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_RELEASE) {
    _keypress->_2 = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_3) == GLFW_PRESS) {
    _keypress->_3 = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_3) == GLFW_RELEASE) {
    _keypress->_3 = false;
  }

  // Spacebar
  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    _keypress->spacebar = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    _keypress->spacebar = false;
  } 
  
  
  // PageUp
  if (glfwGetKey(g_display.window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
    _keypress->pageup = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_PAGE_UP) == GLFW_RELEASE) {
    _keypress->pageup = false;
  }  
  // PageDOwn
  if (glfwGetKey(g_display.window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
    _keypress->pagedown = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE) {
    _keypress->pagedown = false;
  }

}

void Controls::mouseMovement(float x, float y)
{
  static bool firstMouse = true;
  static float lastX(0.f), lastY(0.f), xOffset(0.f), yOffset(0.f);
  if (firstMouse)
  {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }

  xOffset = x - lastX;
  yOffset = lastY - y;

  lastX = x;
  lastY = y;

  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;

  _mousepos->xOffset = xOffset;
  _mousepos->yOffset = yOffset;
}

void Controls::scrollMovement(float x, float y)
{
  _scrolling->xOffset = x;
  _scrolling->yOffset = y;
}

void Controls::setMouse(std::shared_ptr<mouse>& mouse)
{
  _mousepos = mouse;
}

void Controls::setKeyboard(std::shared_ptr<keys>& key)
{
  _keypress = key;
}

void Controls::setScroller(std::shared_ptr<scroll>& scroller)
{
  _scrolling = scroller;
}