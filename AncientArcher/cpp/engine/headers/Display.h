#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
/**
 * display based on glfw3 https://www.glfw.org/docs/latest/group__input.html
 */
class Display {
public:
  enum MouseControlType {
    FPP, SIDESCROLLER
  };

  Display(std::string windowName = "AncientArcherEngine", MouseControlType mouseType = MouseControlType::FPP);
  ~Display();

  GLFWwindow* window;

  void clear() const;
  void update() const;

  int window_width;
  int window_height;

  void reshapeWindow(GLFWwindow* window, int w, int h);
  void FPPmouseHandler(GLFWwindow* window, double xpos, double ypos);
  void SSmouseHandler(GLFWwindow* window, double xpos, double ypos);

  void enableCursor();
  void disableCursor();

private:

  void setupReshapeWindow();

  void setupMouseHandlerToFPPMode();

  void setupMouseHanderToSideScrollerMode();

  // no default constructor allowed
  Display() = delete;
};
