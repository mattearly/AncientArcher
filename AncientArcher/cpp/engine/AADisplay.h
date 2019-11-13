#pragma once
#include "AAControls.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\ext\vector_float3.hpp>

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
  void setWindowSize(int width, int height, int xpos = 0, int ypos = 0);
  void setWindowClearColor(glm::vec3 rgb);
  void setFullscreenToOn();
  void setFullscreenToOff();

  void clearBackBuffer() const;
  void swapWindowBuffers() const;

  bool gWindowRatioChanged = false;
  int getScreenWidth();
  int getScreenHeight();
  bool getIsWindowFullScreen();

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
  glm::vec3 mWindowClearColor = glm::vec3(1.0, 0.0f, 0.0f);
  void initGLFW();
  void initReshapeWindowHandler();
  void initMouseHandler();
  void initMouseScrollHandler();

};

