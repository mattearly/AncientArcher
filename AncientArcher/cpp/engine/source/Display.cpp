#include <Display.h>
#include <Controls.h>
#include <Camera.h>
#include <utility>
#include <iostream>

Controls g_controls;

Display::Display(std::string winName, uint16_t width, uint16_t height, bool fullscreen)
{

  window_width = width;
  window_height = height;

  /* init glfw and opengl and game components */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // init window
  //window = glfwCreateWindow(window_width, window_height, winName.c_str(), nullptr, nullptr);
  window = glfwCreateWindow(window_width, window_height, winName.c_str(), NULL, nullptr);
  if (window == nullptr) {
    std::cout << "failed to create glfw window" << std::endl;
    glfwTerminate();
    char a;
    std::cin >> a;
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  setupReshapeWindow();      // setup reshape window handler
  setupMouseHandler();       // setup the mouse handler
  setupScrollHandler();      // setup the scrollwheel handler

  //enableCursor();
  disableCursor();

  if (fullscreen)
  {
    glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, width, height, NULL);
  }
  else
  {
    glfwSetWindowMonitor(window, NULL, 1920/2 - width/2, 1080/2 - height/2, width, height, NULL);
  }
  // init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to init GLAD" << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

}

Display::~Display() 
{
  glfwTerminate();
}

void Display::reshapeWindow(GLFWwindow* window, uint16_t w, uint16_t h) 
{
  glViewport(0, 0, w, h);
  window_width = w;
  window_height = h;
}

void Display::mouseHandler(GLFWwindow* window, float xpos, float ypos)
{
  g_controls.mouseMovement(xpos, ypos);
}

void Display::scrollHandler(GLFWwindow* window, float xpos, float ypos)
{
  g_controls.scrollMovement(xpos, ypos);
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

void Display::clear() const 
{
  glClearColor(0.1f, 0.1f, 0.85f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::update() const 
{
  glfwSwapBuffers(window);
}


static Display* g_CurrentInstance;

extern "C" void reshapeCallback(GLFWwindow* window, int w, int h) 
{
  g_CurrentInstance->reshapeWindow(window, w, h);
}

extern "C" void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  g_CurrentInstance->mouseHandler(window, (float)xpos, (float)ypos);
}

extern "C" void scrollCallback(GLFWwindow* window, double xpos, double ypos)
{
  g_CurrentInstance->scrollHandler(window, (float)xpos, (float)ypos);
}

void Display::setupReshapeWindow() 
{
  ::g_CurrentInstance = this;
  ::glfwSetFramebufferSizeCallback(window, ::reshapeCallback);
}

void Display::setupMouseHandler() 
{
  ::g_CurrentInstance = this;
  ::glfwSetCursorPosCallback(window, ::mouseCallback);
}

void Display::setupScrollHandler() 
{
  ::g_CurrentInstance = this;
  ::glfwSetScrollCallback(window, ::scrollCallback);
}