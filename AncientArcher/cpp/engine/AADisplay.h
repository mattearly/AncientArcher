#pragma once
#include "AAControls.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm\ext\vector_float3.hpp>

enum class MouseReporting { STANDARD, PERSPECTIVE };

class AADisplay
{
public:

  static AADisplay* getInstance();

  friend class AAEngine;

  ~AADisplay();

  int getWindowWidth();
  int getWindowHeight();
  bool getIsWindowFullScreen();
  GLFWwindow* getWindow();

  void setCursorToVisible();
  void setCursorToHidden();
  void setCursorToDisabled();
  void setWindowClearColor(glm::vec3 rgb);
  void setWindowSize(int width, int height, int xpos, int ypos);
  void setWindowSize(int width, int height, bool center = true);
  void setWindowSize(const char to);
  void setWindowTitle(const char* name);

  void toggleCursor();
  void closeWindow();

  // window handler(s)
  void reshapeWindowHandler(GLFWwindow* window, int width, int height);

  // mouse handler(s)
  void perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos);
  void standardMouseHandler(GLFWwindow* window, float xpos, float ypos);

  // scroll handlers(s)
  void scrollHandler(GLFWwindow* window, float xpos, float ypos);

private:

  // internal functions 
  void toggleFullScreen();
  void setWindowToFullscreen();
  void setFullscreenToOff();

  void setWindowToMaximized();
  void setWindowToFullscreenBorderless();

  void keepWindowOpen();

  void clearBackBuffer() const;
  void swapWindowBuffers() const;

  void setReshapeWindowHandler();

  void setCurorPosToPerspectiveCalc();

  void setCurorPosToStandardCalc();

  void setScrollWheelHandler();

  void initGLFW();

  void initFromEngine();

  void resetStateDataToDefault();

  // state data
  enum class CursorMode { VISIBLE, HIDDEN, FPS } mCursorMode = CursorMode::VISIBLE;

  MouseReporting mMouseReporting = MouseReporting::STANDARD;

  bool mWindowIsFullScreen = false;
  int mWindowWidth = 800, mWindowHeight = 600;
  int mXPos = 0, mYPos = 0;
  glm::vec3 mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);

  GLFWwindow* mWindow = nullptr;

};
