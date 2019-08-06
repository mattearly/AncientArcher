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
    keypress->leftClick = true;
  }
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    keypress->leftClick = false;
  }

  // Right Click
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    keypress->rightClick = true;
  }
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    keypress->rightClick = false;
  }

  // Shift
  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    keypress->leftShift = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    keypress->leftShift = false;
  }

  // WASD
  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_PRESS) {
    keypress->w = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_RELEASE) {
    keypress->w = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_PRESS) {
    keypress->s = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_RELEASE) {
    keypress->s = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_PRESS) {
    keypress->a = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_RELEASE) {
    keypress->a = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_PRESS) {
    keypress->d = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_RELEASE) {
    keypress->d = false;
  }

  // 1, 2, 3
  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_PRESS) {
    keypress->_1 = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_RELEASE) {
    keypress->_1 = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_PRESS) {
    keypress->_2 = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_RELEASE) {
    keypress->_2 = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_3) == GLFW_PRESS) {
    keypress->_3 = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_3) == GLFW_RELEASE) {
    keypress->_3 = false;
  }

  // Spacebar
  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    keypress->spacebar = true;
  }
  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    keypress->spacebar = false;
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

  mousepos->xOffset = xOffset;
  mousepos->yOffset = yOffset;
}

void Controls::scrollMovement(float x, float y)
{
  static float lastX(0.f), lastY(0.f), xOffset(0.f), yOffset(0.f);

  xOffset = x - lastX;
  yOffset = lastY - y;

  lastX = x;
  lastY = y;

  _scrolling->xOffset = xOffset;
  _scrolling->yOffset = yOffset;
}

void Controls::setMouse(std::shared_ptr<mouse>& mouse)
{
  mousepos = mouse;
}

void Controls::setKeyboard(std::shared_ptr<keys>& key)
{
  keypress = key;
}

void Controls::setScroller(std::shared_ptr<scroll>& scroller)
{
  _scrolling = scroller;
}