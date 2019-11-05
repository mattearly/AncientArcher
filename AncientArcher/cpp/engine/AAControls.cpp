#include "AAControls.h"
#include "AADisplay.h"

AAControls* AAControls::getInstance()
{
  static AAControls* controls = new AAControls();
  return controls;
}

void AAControls::mouseMovement(int xpos, int ypos)
{
}

void AAControls::mouseScrollWheelMovement(int xpos, int ypos)
{
}

void AAControls::keyInput(AAKeyInput* keyInput)
{
  //esc
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    keyInput->esc = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE)
  {
    keyInput->esc = false;
  }
  //function keys
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F1) == GLFW_PRESS)
  {
    keyInput->f1 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F1) == GLFW_RELEASE)
  {
    keyInput->f1 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F2) == GLFW_PRESS)
  {
    keyInput->f2 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F2) == GLFW_RELEASE)
  {
    keyInput->f2 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F3) == GLFW_PRESS)
  {
    keyInput->f3 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F3) == GLFW_RELEASE)
  {
    keyInput->f3 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F4) == GLFW_PRESS)
  {
    keyInput->f4 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F4) == GLFW_RELEASE)
  {
    keyInput->f4 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F5) == GLFW_PRESS)
  {
    keyInput->f5 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F5) == GLFW_RELEASE)
  {
    keyInput->f5 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F6) == GLFW_PRESS)
  {
    keyInput->f6 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F6) == GLFW_RELEASE)
  {
    keyInput->f6 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F7) == GLFW_PRESS)
  {
    keyInput->f7 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F7) == GLFW_RELEASE)
  {
    keyInput->f7 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F8) == GLFW_PRESS)
  {
    keyInput->f8 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F8) == GLFW_RELEASE)
  {
    keyInput->f8 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F9) == GLFW_PRESS)
  {
    keyInput->f9 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F9) == GLFW_RELEASE)
  {
    keyInput->f9 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F10) == GLFW_PRESS)
  {
    keyInput->f10 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F10) == GLFW_RELEASE)
  {
    keyInput->f10 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F11) == GLFW_PRESS)
  {
    keyInput->f11 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F11) == GLFW_RELEASE)
  {
    keyInput->f11 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F12) == GLFW_PRESS)
  {
    keyInput->f12 = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F12) == GLFW_RELEASE)
  {
    keyInput->f12 = false;
  }
  //number key row
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
  {
    keyInput->graveAccent = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
  {
    keyInput->graveAccent = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_1) == GLFW_PRESS) 
  {
    keyInput->n1 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_1) == GLFW_RELEASE)
  {
    keyInput->n1 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_2) == GLFW_PRESS) 
  {
    keyInput->n2 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_2) == GLFW_RELEASE)
  {
    keyInput->n2 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_3) == GLFW_PRESS) 
  {
    keyInput->n3 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_3) == GLFW_RELEASE)
  {
    keyInput->n3 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_4) == GLFW_PRESS) 
  {
    keyInput->n4 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_4) == GLFW_RELEASE)
  {
    keyInput->n4 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_5) == GLFW_PRESS) 
  {
    keyInput->n5 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_5) == GLFW_RELEASE)
  {
    keyInput->n5 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_6) == GLFW_PRESS) 
  {
    keyInput->n6 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_6) == GLFW_RELEASE)
  {
    keyInput->n6 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_7) == GLFW_PRESS) 
  {
    keyInput->n7 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_7) == GLFW_RELEASE)
  {
    keyInput->n7 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_8) == GLFW_PRESS) 
  {
    keyInput->n8 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_8) == GLFW_RELEASE)
  {
    keyInput->n8 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_9) == GLFW_PRESS) 
  {
    keyInput->n9 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_9) == GLFW_RELEASE)
  {
    keyInput->n9 = false;
  }  
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_0) == GLFW_PRESS) 
  {
    keyInput->n0 = true;
  } 
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_0) == GLFW_RELEASE)
  {
    keyInput->n0 = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_MINUS) == GLFW_PRESS)
  {
    keyInput->minus = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_MINUS) == GLFW_RELEASE)
  {
    keyInput->minus = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_EQUAL) == GLFW_PRESS)
  {
    keyInput->equal = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_EQUAL) == GLFW_RELEASE)
  {
    keyInput->equal = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSPACE) == GLFW_PRESS)
  {
    keyInput->backspace = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
  {
    keyInput->backspace = false;
  }
  //alphabet keys
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_A) == GLFW_PRESS) 
  {
    keyInput->a = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_A) == GLFW_RELEASE) 
  {
    keyInput->a = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_B) == GLFW_PRESS) 
  {
    keyInput->b = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_B) == GLFW_RELEASE) 
  {
    keyInput->b = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_C) == GLFW_PRESS) 
  {
    keyInput->c = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_C) == GLFW_RELEASE) 
  {
    keyInput->c = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_D) == GLFW_PRESS) 
  {
    keyInput->d = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_D) == GLFW_RELEASE) 
  {
    keyInput->d = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_E) == GLFW_PRESS) 
  {
    keyInput->e = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_E) == GLFW_RELEASE) 
  {
    keyInput->e = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F) == GLFW_PRESS) 
  {
    keyInput->f = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_F) == GLFW_RELEASE) 
  {
    keyInput->f = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_G) == GLFW_PRESS) 
  {
    keyInput->g = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_G) == GLFW_RELEASE) 
  {
    keyInput->g = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_H) == GLFW_PRESS) 
  {
    keyInput->h = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_H) == GLFW_RELEASE) 
  {
    keyInput->h = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_I) == GLFW_PRESS) 
  {
    keyInput->i = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_I) == GLFW_RELEASE) 
  {
    keyInput->i = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_J) == GLFW_PRESS) 
  {
    keyInput->j = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_J) == GLFW_RELEASE) 
  {
    keyInput->j = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_K) == GLFW_PRESS) 
  {
    keyInput->k = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_K) == GLFW_RELEASE) 
  {
    keyInput->k = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_L) == GLFW_PRESS) 
  {
    keyInput->l = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_L) == GLFW_RELEASE) 
  {
    keyInput->l = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_M) == GLFW_PRESS) 
  {
    keyInput->m = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_M) == GLFW_RELEASE) 
  {
    keyInput->m = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_N) == GLFW_PRESS) 
  {
    keyInput->n = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_N) == GLFW_RELEASE) 
  {
    keyInput->n = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_O) == GLFW_PRESS) 
  {
    keyInput->o = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_O) == GLFW_RELEASE) 
  {
    keyInput->o = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_P) == GLFW_PRESS) 
  {
    keyInput->p = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_P) == GLFW_RELEASE) 
  {
    keyInput->p = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Q) == GLFW_PRESS) 
  {
    keyInput->q = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Q) == GLFW_RELEASE) 
  {
    keyInput->q = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_R) == GLFW_PRESS) 
  {
    keyInput->r = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_R) == GLFW_RELEASE) 
  {
    keyInput->r = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_S) == GLFW_PRESS) 
  {
    keyInput->s = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_S) == GLFW_RELEASE) 
  {
    keyInput->s = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_T) == GLFW_PRESS) 
  {
    keyInput->t = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_T) == GLFW_RELEASE) 
  {
    keyInput->t = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_U) == GLFW_PRESS) 
  {
    keyInput->u = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_U) == GLFW_RELEASE) 
  {
    keyInput->u = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_V) == GLFW_PRESS) 
  {
    keyInput->v = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_V) == GLFW_RELEASE) 
  {
    keyInput->v = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_W) == GLFW_PRESS) 
  {
    keyInput->w = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_W) == GLFW_RELEASE) 
  {
    keyInput->w = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_X) == GLFW_PRESS) 
  {
    keyInput->x = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_X) == GLFW_RELEASE) 
  {
    keyInput->x = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Y) == GLFW_PRESS) 
  {
    keyInput->y = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Y) == GLFW_RELEASE) 
  {
    keyInput->y = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Z) == GLFW_PRESS) 
  {
    keyInput->z = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_Z) == GLFW_RELEASE) 
  {
    keyInput->z = false;
  }
  //tab-shift-control-alt
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_TAB) == GLFW_PRESS)
  {
    keyInput->tab = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_TAB) == GLFW_RELEASE)
  {
    keyInput->tab = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    keyInput->leftShift = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    keyInput->leftShift = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
  {
    keyInput->rightShift = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
  {
    keyInput->rightShift = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    keyInput->leftControl = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
  {
    keyInput->leftControl = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
  {
    keyInput->rightControl = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE)
  {
    keyInput->rightControl = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
  {
    keyInput->leftAlt = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
  {
    keyInput->leftAlt = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
  {
    keyInput->rightAlt = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE)
  {
    keyInput->rightAlt = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    keyInput->spacebar = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
    keyInput->spacebar = false;
  }
  //brackets
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
  {
    keyInput->leftSquareBracket = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
  {
    keyInput->leftSquareBracket = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
  {
    keyInput->rightSquareBracket = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
  {
    keyInput->rightSquareBracket = false;
  }
  //slash-quote-semicolon-enter
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSLASH) == GLFW_PRESS)
  {
    keyInput->backslash = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_BACKSLASH) == GLFW_RELEASE)
  {
    keyInput->backslash = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SEMICOLON) == GLFW_PRESS)
  {
    keyInput->semiColon = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
  {
    keyInput->semiColon = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
  {
    keyInput->apostrophe = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_APOSTROPHE) == GLFW_RELEASE)
  {
    keyInput->apostrophe = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    keyInput->enter = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_ENTER) == GLFW_RELEASE)
  {
    keyInput->enter = false;
  }
  //comma-period-forwardslash
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_COMMA) == GLFW_PRESS)
  {
    keyInput->comma = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_COMMA) == GLFW_RELEASE)
  {
    keyInput->comma = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PERIOD) == GLFW_PRESS)
  {
    keyInput->period = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PERIOD) == GLFW_RELEASE)
  {
    keyInput->period = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SLASH) == GLFW_PRESS)
  {
    keyInput->forwardSlash = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SLASH) == GLFW_RELEASE)
  {
    keyInput->forwardSlash = false;
  }
  //printscreen-etc
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS)
  {
    keyInput->printScreen = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PRINT_SCREEN) == GLFW_RELEASE)
  {
    keyInput->printScreen = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS)
  {
    keyInput->scrollLock = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_SCROLL_LOCK) == GLFW_RELEASE)
  {
    keyInput->scrollLock = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAUSE) == GLFW_PRESS)
  {
    keyInput->pauseBreak = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAUSE) == GLFW_RELEASE)
  {
    keyInput->pauseBreak = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_INSERT) == GLFW_PRESS)
  {
    keyInput->insert = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_INSERT) == GLFW_RELEASE)
  {
    keyInput->insert = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DELETE) == GLFW_PRESS)
  {
    keyInput->del = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DELETE) == GLFW_RELEASE)
  {
    keyInput->del = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_HOME) == GLFW_PRESS)
  {
    keyInput->home = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_HOME) == GLFW_RELEASE)
  {
    keyInput->home = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_END) == GLFW_PRESS)
  {
    keyInput->end = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_END) == GLFW_RELEASE)
  {
    keyInput->end = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
    keyInput->pageUp = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_UP) == GLFW_RELEASE) {
    keyInput->pageUp = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
    keyInput->pageDown = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE) {
    keyInput->pageDown = false;
  }
  //arrows
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
    keyInput->upArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_UP) == GLFW_RELEASE) {
    keyInput->upArrow = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
    keyInput->downArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_DOWN) == GLFW_RELEASE) {
    keyInput->downArrow = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
    keyInput->leftArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_LEFT) == GLFW_RELEASE) {
    keyInput->leftArrow = false;
  }
  if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
    keyInput->rightArrow = true;
  }
  else if (glfwGetKey(AADisplay::getInstance()->getWindow(), GLFW_KEY_RIGHT) == GLFW_RELEASE) {
    keyInput->rightArrow = false;
  }

  //mouse clicks
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    keyInput->mouseButton1 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    keyInput->mouseButton1 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    keyInput->mouseButton2 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    keyInput->mouseButton2 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
    keyInput->mouseButton3 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
    keyInput->mouseButton3 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_4) == GLFW_PRESS) {
    keyInput->mouseButton4 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_4) == GLFW_RELEASE) {
    keyInput->mouseButton4 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_5) == GLFW_PRESS) {
    keyInput->mousebutton5 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_5) == GLFW_RELEASE) {
    keyInput->mousebutton5 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_6) == GLFW_PRESS) {
    keyInput->mouseButton6 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_6) == GLFW_RELEASE) {
    keyInput->mouseButton6 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_7) == GLFW_PRESS) {
    keyInput->mousebutton7 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_7) == GLFW_RELEASE) {
    keyInput->mousebutton7 = false;
  }
  if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_8) == GLFW_PRESS) {
    keyInput->mouseButton8 = true;
  }
  else if (glfwGetMouseButton(AADisplay::getInstance()->getWindow(), GLFW_MOUSE_BUTTON_8) == GLFW_RELEASE) {
    keyInput->mouseButton8 = false;
  }




}
