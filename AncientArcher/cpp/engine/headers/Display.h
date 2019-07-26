#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <string>
/**
 * display based on glfw3 https://www.glfw.org/docs/latest/group__input.html
 */
class Display {
public:

  Display(std::string windowName = "AAEngine");
  ~Display();

  GLFWwindow* window;

  void clear() const;
  void update() const;

  int window_width;
  int window_height;

  void reshapeWindow(GLFWwindow* window, int w, int h);
  void mouseHandler(GLFWwindow* window, double xpos, double ypos);

  void enableCursor();
  void disableCursor();

private:

  void setupReshapeWindow();

  void setupMouseHandler();

  // no default constructor allowed
  Display() = delete;
};
