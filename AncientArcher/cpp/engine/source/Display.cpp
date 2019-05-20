#include <Display.h>
#include <Controls.h>
#include <utility>
#include <iostream>

Controls controls;

Display::Display(std::string windowName, MouseControlType mouseType) 
{
  
  window_width = 1280;
  window_height = 720;

  /* init glfw and opengl and game components */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // init window
  window = glfwCreateWindow(window_width, window_height, windowName.c_str(), nullptr, nullptr);
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
  if (mouseType == MouseControlType::FPP)
  {
    setupMouseHandlerToFPPMode();
    disableCursor();
  }
  else  // SIDESCROLLER MOUSE
  {
    setupMouseHanderToSideScrollerMode();
    enableCursor();
  }

  // init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to init GLAD" << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

}

Display::~Display() {
  glfwTerminate();
}

void Display::reshapeWindow(GLFWwindow * window, int w, int h) {
  glViewport(0, 0, w, h);
  window_width = w;
  window_height = h;
}

void Display::FPPmouseHandler(GLFWwindow * window, double xpos, double ypos) {
  controls.FPPmouseMovement((float)xpos, (float)ypos);
}

void Display::SSmouseHandler(GLFWwindow * window, double xpos, double ypos) {
  controls.SSmouseMovement((float)xpos, (float)ypos);
}

/**
 * GLFW_CURSOR_NORMAL makes the cursor visible and behaving normally.
 */
void Display::enableCursor()
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/**
 * GLFW_CURSOR_DISABLED hides and grabs the cursor, providing virtual and unlimited cursor movement. 
 * This is useful for implementing for example 3D camera controls.
 */
void Display::disableCursor()
{
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Display::clear() const {
  glClearColor(0.1f, 0.1f, 0.85f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::update() const {
  glfwSwapBuffers(window);
}

static Display* g_CurrentInstance;

extern "C" void reshapeCallback(GLFWwindow * window, int w, int h) {
  g_CurrentInstance->reshapeWindow(window, w, h);
}

/**
 * First Person mouse.
 */
extern "C" void mouseCallbackFPP(GLFWwindow * window, double xpos, double ypos) {
  g_CurrentInstance->FPPmouseHandler(window, xpos, ypos);
}

/**
 * Sidescroller mouse.
 */
extern "C" void SSmouseCallback(GLFWwindow * window, double xpos, double ypos) {
  g_CurrentInstance->SSmouseHandler(window, xpos, ypos);
}

void Display::setupReshapeWindow() {
  ::g_CurrentInstance = this;
  ::glfwSetFramebufferSizeCallback(window, ::reshapeCallback);
}

void Display::setupMouseHandlerToFPPMode() {
  ::g_CurrentInstance = this;
  ::glfwSetCursorPosCallback(window, ::mouseCallbackFPP);
}

void Display::setupMouseHanderToSideScrollerMode()
{
  ::g_CurrentInstance = this;
  ::glfwSetCursorPosCallback(window, ::SSmouseCallback);
}