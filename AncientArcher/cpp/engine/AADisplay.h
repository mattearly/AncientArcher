#pragma once
#include "AAControls.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AADisplay
{

public:
  static AADisplay* getInstance();

  AADisplay();
  ~AADisplay();

  void reshapeWindowHandler(GLFWwindow* window, int width, int height);
  void mouseHandler(GLFWwindow* window, float xpos, float ypos);
  void scrollHandler(GLFWwindow* window, float xpos, float ypos);

  void enableCursor();
  void disableCursor();

  void setWindowTitle(const char* name);  
  void FullscreenOn();
  void FullscreenOff();

  void clearBackBuffer() const;
  void swapWindowBuffers() const;

  bool gWindowNeedsResized = false;

  GLFWwindow* getWindow();

private:

  int mWindowWidth, mWindowHeight;
  GLFWmonitor* mMonitor;
  GLFWwindow* mWindow;

  void initGLFW();
  void initReshapeWindowHandler();
  void initMouseHandler();
  void initMouseScrollHandler();
  
};

