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

  Display(std::string winName, uint16_t width = 1280, uint16_t height = 720, bool fullscreen = false);
  ~Display();

  GLFWwindow* window;

  void clear() const;
  void update() const;
  
  int window_width;
  int window_height;

  void reshapeWindow(GLFWwindow* window, uint16_t w, uint16_t h);
  void mouseHandler(GLFWwindow* window, float xpos, float ypos);

  void enableCursor();
  void disableCursor();

private:

  Display() = delete;    // no default constructor

  void setupReshapeWindow();

  void setupMouseHandler();

};
