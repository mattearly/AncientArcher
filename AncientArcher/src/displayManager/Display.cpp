#include "Display.h"
#include <utility>
#include <iostream>

Display::Display() {

  window_width = 1024;
  window_height = 768;

  /* init glfw and opengl and game components */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


  // init window
  window = glfwCreateWindow(window_width, window_height, "AA", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "failed to create glfw window" << std::endl;
    glfwTerminate();
    char a;
    std::cin >> a;
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  // setup reshape window handler
  setupReshapeWindow();

  // setup mouse handler
  setupMouseHandler();

  // init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to init GLAD" << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  // make cursor hidden
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

Display::~Display() {}

void Display::reshapeWindow(GLFWwindow * window, int w, int h) {
  glViewport(0, 0, w, h);
  window_width = w;
  window_height = h;
}

void Display::mouseHandler(GLFWwindow * window, double xpos, double ypos) {

  //control->mouseMovement(xpos, ypos, camera);

  //return std::pair<double, double>(xpos, ypos);

  //debug
  std::cout << "mouse (x, y): " << xpos << ", " << ypos << "\n";

}

void Display::update() {
  glfwSwapBuffers(window);
}

static Display * g_CurrentInstance;

extern "C" void reshapeCallback(GLFWwindow *window, int w, int h) {
  g_CurrentInstance->reshapeWindow(window, w, h);
}

extern "C" void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  g_CurrentInstance->mouseHandler(window, xpos, ypos);
}

void Display::setupReshapeWindow() {
  ::g_CurrentInstance = this;
  ::glfwSetFramebufferSizeCallback(window, ::reshapeCallback);
}

void Display::setupMouseHandler() {
  ::g_CurrentInstance = this;
  ::glfwSetCursorPosCallback(window, ::mouseCallback);
}
