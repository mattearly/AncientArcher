#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class MouseReporting { STANDARD, PERSPECTIVE };

class AADisplay
{
public:

  static AADisplay* getInstance();

  friend class AAEngine;

  ~AADisplay();

  int getWindowWidth() noexcept;
  int getWindowHeight() noexcept;
  bool getIsWindowFullScreen() noexcept;
  GLFWwindow* getWindow() noexcept;
  int getCursorMode() const noexcept;
  MouseReporting getMouseReportingMode() const noexcept;

  void setCursorToVisible() noexcept;   // to use the os pointer
  void setCursorToHidden() noexcept;    // for hidden but still there (render your own pointer)
  void setCursorToDisabled() noexcept;  // for first person hidden mouse type

  void setWindowClearColor(glm::vec3 rgb) noexcept;
  void setWindowSize(int width, int height, int xpos, int ypos) noexcept;
  void setWindowSize(int width, int height, bool center = true) noexcept;
  void setWindowSize(const char to) noexcept;
  void setWindowTitle(const char* name) noexcept;

  void closeWindow() noexcept;

  // PUBLIC HANDLERS FOR GLFW
  void reshapeWindowHandler(GLFWwindow* window, int width, int height);
  void perspectiveMouseHandler(GLFWwindow* window, float xpos, float ypos);
  void standardMouseHandler(GLFWwindow* window, float xpos, float ypos);
  void scrollHandler(GLFWwindow* window, float xpos, float ypos);

  friend class AAWorld;

private:

  // internal functions 
  void toggleFullScreen() noexcept;
  void setWindowToFullscreen() noexcept;
  void setFullscreenToOff() noexcept;

  void setWindowToMaximized() noexcept;
  void setWindowToFullscreenBorderless() noexcept;

  void keepWindowOpen() noexcept;

  void clearBackBuffer() const noexcept;
  void swapWindowBuffers() const noexcept;

  void setReshapeWindowHandler() noexcept;
  void setCurorPosToPerspectiveCalc() noexcept;
  void setCurorPosToStandardCalc() noexcept;
  void setScrollWheelHandler() noexcept;

  void initGLFW() noexcept;
  void initFromEngine();
  void resetStateDataToDefault();

  // state data
  MouseReporting mMouseReporting = MouseReporting::STANDARD;
  bool mWindowIsFullScreen = false;
  int mWindowWidth = 800, mWindowHeight = 600;
  int mXPos = 0, mYPos = 0;
  glm::vec3 mWindowClearColor = glm::vec3(0.35f, 0.15f, 0.35f);
  GLFWwindow* mWindow = nullptr;

  // hold for engine to change back to false  so it can know if it should resize its viewport
  bool mWindowSizeChanged = true;

};
