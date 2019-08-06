#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <keys.h>
#include <mouse.h>
#include <scroll.h>
#include <memory>

class Controls {

public:

  void mouseMovement(float x, float y);

  void scrollMovement(float x, float y);

  void keyboardInput();
 
  void setKeyboard(std::shared_ptr<keys>& kb);
  void setMouse(std::shared_ptr<mouse>& mouse);
  void setScroller(std::shared_ptr<scroll>& sroller);

private:

  float mouseSensitivity = 0.09f;

  std::shared_ptr<keys> keypress;
  std::shared_ptr<mouse> mousepos;
  std::shared_ptr<scroll> _scrolling;

};
