#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Display {
public:
  Display();
  ~Display();

  GLFWwindow *window;

  void clear() const;
  void update() const;

  int window_width;
  int window_height;

  void reshapeWindow(GLFWwindow *window, int w, int h);
  void mouseHandler(GLFWwindow *window, double xpos, double ypos);

private:

  void setupReshapeWindow();
  void setupMouseHandler();


};
