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

  bool gWindowRatioChanged = false;
  int getScreenWidth();
  int getScreenHeight();

  GLFWwindow* getWindow();

private:

  bool mWindowIsFullScreen = false;
  int mWindowWidth = 0, mWindowHeight = 0;
  int mXPos = 0, mYPos = 0;
  GLFWmonitor* mMonitor;
  GLFWwindow* mWindow;
  int mLaunchedMainScreenWindowWidth = 0;
  int mLaunchedMainScreenWindowHeight = 0;
  int mWindowFrameSizeLeft = 0, mWindowFrameSizeTop = 0, mWindowFrameSizeRight = 0, mWindowFrameSizeBottom = 0;

  void initGLFW();
  void initReshapeWindowHandler();
  void initMouseHandler();
  void initMouseScrollHandler();
  
};

