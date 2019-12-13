#pragma once
#include "AAControls.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\ext\vector_float3.hpp>

class AADisplay
{

public:

  static AADisplay* getInstance();

  ~AADisplay();

  void reshapeWindowHandler(GLFWwindow* window, int width, int height);
  void mouseHandler(GLFWwindow* window, float xpos, float ypos);
  void scrollHandler(GLFWwindow* window, float xpos, float ypos);

  void setCursorToVisible();
  void setCursorToHidden();
  void setCursorToDisabled();

  void setWindowTitle(const char* name);
  void setWindowSize(int width, int height, int xpos = 0, int ypos = 0);
  void setWindowClearColor(glm::vec3 rgb);

  int getScreenWidth();
  int getScreenHeight();
  bool getIsWindowFullScreen();
  GLFWwindow* getWindow();

  friend class AAEngine;

private:

  void toggleFullScreen();
  void setFullscreenToOn();
  void setFullscreenToOff();

  void clearBackBuffer() const;
  void swapWindowBuffers() const;

  void keepWindowOpen();
  void closeWindow();

  bool mWindowIsFullScreen = false;
  int mWindowWidth = 800, mWindowHeight = 600;
  int mXPos = 0, mYPos = 0;
  //GLFWmonitor* mMonitor = nullptr;
  GLFWwindow* mWindow = nullptr;
  glm::vec3 mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);

  //int mLaunchedMainScreenWindowWidth = 0;
  //int mLaunchedMainScreenWindowHeight = 0;
  //int mWindowFrameSizeLeft = 0, mWindowFrameSizeTop = 0, mWindowFrameSizeRight = 0, mWindowFrameSizeBottom = 0;

  void initGLFW();
  void initReshapeWindowHandler();
  void initMouseHandler();
  void initMouseScrollHandler();

  void initFromEngine();
};

