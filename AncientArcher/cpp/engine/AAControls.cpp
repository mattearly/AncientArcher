#include "AAControls.h"
#include "AADisplay.h"
#include <iostream>

AAControls* AAControls::getInstance()
{
  static AAControls* controls = new AAControls();
  return controls;
}

void AAControls::perspectiveMouseMovement(float x, float y)
{
  //first person
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

  xOffset *= mMouseSensitivity;
  yOffset *= mMouseSensitivity;

  mMousePosition.xOffset = xOffset;
  mMousePosition.yOffset = yOffset;

}

void AAControls::standardMouseMovement(float xpos, float ypos)
{
  mMousePosition.xOffset = xpos;
  mMousePosition.yOffset = ypos;
}

void AAControls::mouseScrollWheelMovement(float x, float y)
{
  mMouseWheelScroll.xOffset = x;
  mMouseWheelScroll.yOffset = y;
}

void AAControls::pullButtonStateEvents()
{
  glfwPollEvents();

  // esc
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    mButtonState.esc = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE)
  {
    mButtonState.esc = false;
  }
  // function keys
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
  {
    mButtonState.f1 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F1) == GLFW_RELEASE)
  {
    mButtonState.f1 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F2) == GLFW_PRESS)
  {
    mButtonState.f2 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F2) == GLFW_RELEASE)
  {
    mButtonState.f2 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F3) == GLFW_PRESS)
  {
    mButtonState.f3 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F3) == GLFW_RELEASE)
  {
    mButtonState.f3 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F4) == GLFW_PRESS)
  {
    mButtonState.f4 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F4) == GLFW_RELEASE)
  {
    mButtonState.f4 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F5) == GLFW_PRESS)
  {
    mButtonState.f5 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F5) == GLFW_RELEASE)
  {
    mButtonState.f5 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F6) == GLFW_PRESS)
  {
    mButtonState.f6 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F6) == GLFW_RELEASE)
  {
    mButtonState.f6 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F7) == GLFW_PRESS)
  {
    mButtonState.f7 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F7) == GLFW_RELEASE)
  {
    mButtonState.f7 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F8) == GLFW_PRESS)
  {
    mButtonState.f8 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F8) == GLFW_RELEASE)
  {
    mButtonState.f8 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F9) == GLFW_PRESS)
  {
    mButtonState.f9 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F9) == GLFW_RELEASE)
  {
    mButtonState.f9 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F10) == GLFW_PRESS)
  {
    mButtonState.f10 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F10) == GLFW_RELEASE)
  {
    mButtonState.f10 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F11) == GLFW_PRESS)
  {
    mButtonState.f11 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F11) == GLFW_RELEASE)
  {
    mButtonState.f11 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F12) == GLFW_PRESS)
  {
    mButtonState.f12 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F12) == GLFW_RELEASE)
  {
    mButtonState.f12 = false;
  }
  // number key row
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
  {
    mButtonState.graveAccent = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
  {
    mButtonState.graveAccent = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_1) == GLFW_PRESS) 
  {
    mButtonState.n1 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_1) == GLFW_RELEASE)
  {
    mButtonState.n1 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_2) == GLFW_PRESS) 
  {
    mButtonState.n2 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_2) == GLFW_RELEASE)
  {
    mButtonState.n2 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_3) == GLFW_PRESS) 
  {
    mButtonState.n3 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_3) == GLFW_RELEASE)
  {
    mButtonState.n3 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_4) == GLFW_PRESS) 
  {
    mButtonState.n4 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_4) == GLFW_RELEASE)
  {
    mButtonState.n4 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_5) == GLFW_PRESS) 
  {
    mButtonState.n5 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_5) == GLFW_RELEASE)
  {
    mButtonState.n5 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_6) == GLFW_PRESS) 
  {
    mButtonState.n6 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_6) == GLFW_RELEASE)
  {
    mButtonState.n6 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_7) == GLFW_PRESS) 
  {
    mButtonState.n7 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_7) == GLFW_RELEASE)
  {
    mButtonState.n7 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_8) == GLFW_PRESS) 
  {
    mButtonState.n8 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_8) == GLFW_RELEASE)
  {
    mButtonState.n8 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_9) == GLFW_PRESS) 
  {
    mButtonState.n9 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_9) == GLFW_RELEASE)
  {
    mButtonState.n9 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_0) == GLFW_PRESS) 
  {
    mButtonState.n0 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_0) == GLFW_RELEASE)
  {
    mButtonState.n0 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_MINUS) == GLFW_PRESS)
  {
    mButtonState.minus = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_MINUS) == GLFW_RELEASE)
  {
    mButtonState.minus = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_EQUAL) == GLFW_PRESS)
  {
    mButtonState.equal = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_EQUAL) == GLFW_RELEASE)
  {
    mButtonState.equal = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSPACE) == GLFW_PRESS)
  {
    mButtonState.backspace = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
  {
    mButtonState.backspace = false;
  }
  // alphabet keys
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_A) == GLFW_PRESS) 
  {
    mButtonState.a = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_A) == GLFW_RELEASE) 
  {
    mButtonState.a = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_B) == GLFW_PRESS) 
  {
    mButtonState.b = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_B) == GLFW_RELEASE) 
  {
    mButtonState.b = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_C) == GLFW_PRESS) 
  {
    mButtonState.c = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_C) == GLFW_RELEASE) 
  {
    mButtonState.c = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_D) == GLFW_PRESS) 
  {
    mButtonState.d = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_D) == GLFW_RELEASE) 
  {
    mButtonState.d = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_E) == GLFW_PRESS) 
  {
    mButtonState.e = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_E) == GLFW_RELEASE) 
  {
    mButtonState.e = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F) == GLFW_PRESS) 
  {
    mButtonState.f = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F) == GLFW_RELEASE) 
  {
    mButtonState.f = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_G) == GLFW_PRESS) 
  {
    mButtonState.g = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_G) == GLFW_RELEASE) 
  {
    mButtonState.g = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_H) == GLFW_PRESS) 
  {
    mButtonState.h = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_H) == GLFW_RELEASE) 
  {
    mButtonState.h = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_I) == GLFW_PRESS) 
  {
    mButtonState.i = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_I) == GLFW_RELEASE) 
  {
    mButtonState.i = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_J) == GLFW_PRESS) 
  {
    mButtonState.j = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_J) == GLFW_RELEASE) 
  {
    mButtonState.j = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_K) == GLFW_PRESS) 
  {
    mButtonState.k = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_K) == GLFW_RELEASE) 
  {
    mButtonState.k = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_L) == GLFW_PRESS) 
  {
    mButtonState.l = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_L) == GLFW_RELEASE) 
  {
    mButtonState.l = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_M) == GLFW_PRESS) 
  {
    mButtonState.m = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_M) == GLFW_RELEASE) 
  {
    mButtonState.m = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_N) == GLFW_PRESS) 
  {
    mButtonState.n = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_N) == GLFW_RELEASE) 
  {
    mButtonState.n = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_O) == GLFW_PRESS) 
  {
    mButtonState.o = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_O) == GLFW_RELEASE) 
  {
    mButtonState.o = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_P) == GLFW_PRESS) 
  {
    mButtonState.p = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_P) == GLFW_RELEASE) 
  {
    mButtonState.p = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Q) == GLFW_PRESS) 
  {
    mButtonState.q = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Q) == GLFW_RELEASE) 
  {
    mButtonState.q = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_R) == GLFW_PRESS) 
  {
    mButtonState.r = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_R) == GLFW_RELEASE) 
  {
    mButtonState.r = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_S) == GLFW_PRESS) 
  {
    mButtonState.s = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_S) == GLFW_RELEASE) 
  {
    mButtonState.s = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_T) == GLFW_PRESS) 
  {
    mButtonState.t = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_T) == GLFW_RELEASE) 
  {
    mButtonState.t = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_U) == GLFW_PRESS) 
  {
    mButtonState.u = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_U) == GLFW_RELEASE) 
  {
    mButtonState.u = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_V) == GLFW_PRESS) 
  {
    mButtonState.v = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_V) == GLFW_RELEASE) 
  {
    mButtonState.v = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_W) == GLFW_PRESS) 
  {
    mButtonState.w = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_W) == GLFW_RELEASE) 
  {
    mButtonState.w = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_X) == GLFW_PRESS) 
  {
    mButtonState.x = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_X) == GLFW_RELEASE) 
  {
    mButtonState.x = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Y) == GLFW_PRESS) 
  {
    mButtonState.y = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Y) == GLFW_RELEASE) 
  {
    mButtonState.y = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Z) == GLFW_PRESS) 
  {
    mButtonState.z = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Z) == GLFW_RELEASE) 
  {
    mButtonState.z = false;
  }
  // tab-shift-control-alt
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_TAB) == GLFW_PRESS)
  {
    mButtonState.tab = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_TAB) == GLFW_RELEASE)
  {
    mButtonState.tab = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    mButtonState.leftShift = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    mButtonState.leftShift = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
  {
    mButtonState.rightShift = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
  {
    mButtonState.rightShift = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    mButtonState.leftControl = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
  {
    mButtonState.leftControl = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
  {
    mButtonState.rightControl = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE)
  {
    mButtonState.rightControl = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
  {
    mButtonState.leftAlt = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
  {
    mButtonState.leftAlt = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
  {
    mButtonState.rightAlt = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE)
  {
    mButtonState.rightAlt = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    mButtonState.spacebar = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
    mButtonState.spacebar = false;
  }
  // brackets
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
  {
    mButtonState.leftSquareBracket = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
  {
    mButtonState.leftSquareBracket = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
  {
    mButtonState.rightSquareBracket = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
  {
    mButtonState.rightSquareBracket = false;
  }
  // slash-quote-semicolon-enter
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSLASH) == GLFW_PRESS)
  {
    mButtonState.backslash = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSLASH) == GLFW_RELEASE)
  {
    mButtonState.backslash = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SEMICOLON) == GLFW_PRESS)
  {
    mButtonState.semiColon = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
  {
    mButtonState.semiColon = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
  {
    mButtonState.apostrophe = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_APOSTROPHE) == GLFW_RELEASE)
  {
    mButtonState.apostrophe = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    mButtonState.enter = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ENTER) == GLFW_RELEASE)
  {
    mButtonState.enter = false;
  }
  // comma-period-forwardslash
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_COMMA) == GLFW_PRESS)
  {
    mButtonState.comma = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_COMMA) == GLFW_RELEASE)
  {
    mButtonState.comma = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PERIOD) == GLFW_PRESS)
  {
    mButtonState.period = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PERIOD) == GLFW_RELEASE)
  {
    mButtonState.period = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SLASH) == GLFW_PRESS)
  {
    mButtonState.forwardSlash = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SLASH) == GLFW_RELEASE)
  {
    mButtonState.forwardSlash = false;
  }
  // printscreen-etc
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS)
  {
    mButtonState.printScreen = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PRINT_SCREEN) == GLFW_RELEASE)
  {
    mButtonState.printScreen = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS)
  {
    mButtonState.scrollLock = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SCROLL_LOCK) == GLFW_RELEASE)
  {
    mButtonState.scrollLock = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAUSE) == GLFW_PRESS)
  {
    mButtonState.pauseBreak = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAUSE) == GLFW_RELEASE)
  {
    mButtonState.pauseBreak = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_INSERT) == GLFW_PRESS)
  {
    mButtonState.insert = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_INSERT) == GLFW_RELEASE)
  {
    mButtonState.insert = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DELETE) == GLFW_PRESS)
  {
    mButtonState.del = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DELETE) == GLFW_RELEASE)
  {
    mButtonState.del = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_HOME) == GLFW_PRESS)
  {
    mButtonState.home = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_HOME) == GLFW_RELEASE)
  {
    mButtonState.home = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_END) == GLFW_PRESS)
  {
    mButtonState.end = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_END) == GLFW_RELEASE)
  {
    mButtonState.end = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
    mButtonState.pageUp = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_UP) == GLFW_RELEASE) {
    mButtonState.pageUp = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
    mButtonState.pageDown = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE) {
    mButtonState.pageDown = false;
  }
  // arrows
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
    mButtonState.upArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_UP) == GLFW_RELEASE) {
    mButtonState.upArrow = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
    mButtonState.downArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DOWN) == GLFW_RELEASE) {
    mButtonState.downArrow = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
    mButtonState.leftArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT) == GLFW_RELEASE) {
    mButtonState.leftArrow = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
    mButtonState.rightArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT) == GLFW_RELEASE) {
    mButtonState.rightArrow = false;
  }

  // mouse clicks
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    mButtonState.mouseButton1 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    mButtonState.mouseButton1 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    mButtonState.mouseButton2 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    mButtonState.mouseButton2 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
    mButtonState.mouseButton3 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
    mButtonState.mouseButton3 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_4) == GLFW_PRESS) {
    mButtonState.mouseButton4 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_4) == GLFW_RELEASE) {
    mButtonState.mouseButton4 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_5) == GLFW_PRESS) {
    mButtonState.mousebutton5 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_5) == GLFW_RELEASE) {
    mButtonState.mousebutton5 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_6) == GLFW_PRESS) {
    mButtonState.mouseButton6 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_6) == GLFW_RELEASE) {
    mButtonState.mouseButton6 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_7) == GLFW_PRESS) {
    mButtonState.mousebutton7 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_7) == GLFW_RELEASE) {
    mButtonState.mousebutton7 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_8) == GLFW_PRESS) {
    mButtonState.mouseButton8 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_8) == GLFW_RELEASE) {
    mButtonState.mouseButton8 = false;
  }
}
