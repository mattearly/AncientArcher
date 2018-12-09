#include "Game.h"
#include "Controls.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Game::Game() {

  /* init glfw and opengl and game components */
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  /* init window */
  window = glfwCreateWindow(window_width, window_height, "TITLE", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "failed to create glfw window" << std::endl;
    glfwTerminate();
    char a;
    std::cin >> a;
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  setupReshapeWindow();
  setupMouseHandler();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "failed to init GLAD" << std::endl;
    char a;
    std::cin >> a;
    exit(-1);
  }

  control = new Controls();
  shader = new Shader("../AncientArcher/shaders/vertex.shader", "../AncientArcher/shaders/fragment.shader");
  lightShader = new Shader("../AncientArcher/shaders/lightvertex.shader", "../AncientArcher/shaders/lightfragment.shader");
  camera = new Camera();  // init Camera with all defaults

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

     // cube
  float vertices[] = {
    // positions          // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  // set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
  float lightingNormals[] = {
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,
      0.0f,  0.0f, -1.0f,

      0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,
      0.0f,  0.0f,  1.0f,

      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,

      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,
      1.0f,  0.0f,  0.0f,

      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,
      0.0f, -1.0f,  0.0f,

      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f,
      0.0f,  1.0f,  0.0f
  };


  /* set up an area to store a vertex data */
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  //glGenBuffers(1, &EBO);

  glBindVertexArray(cubeVAO);

  /* prep to send to send vertex buffer object */
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // lighting attribute in it's own vertex array
  glGenVertexArrays(1, &lightVAO);
  glGenBuffers(1, &lightVBO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lightingNormals), lightingNormals, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  /* preload some textures */
  texBank.loadTexture("../AncientArcher/res/texture/grass_mosaic.png", shader);

  /* update projection matrix  - this may rarely chance and could be more efficient
     by not being in the main loop */
  glm::mat4 projection = glm::perspective(glm::radians(camera->FoV),
    (float)window_width / (float)window_height, 0.1f, 200.0f);
  shader->setMat4("projection", projection);

  glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind VBO
  glBindVertexArray(0);  //unbind VAO

  glEnable(GL_DEPTH_TEST);

}

Game::~Game() {

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);

  glfwTerminate();

 }

void Game::reshapeWindow(GLFWwindow * window, int w, int h) {
  glViewport(0, 0, w, h);
}

void Game::mouseHandler(GLFWwindow * window, double xpos, double ypos) {
  control->mouseMovement(xpos, ypos, camera);
}

static Game * g_CurrentInstance;

extern "C" void reshapeCallback(GLFWwindow *window, int w, int h) {
  window_width = w;
  window_height = h;
  g_CurrentInstance->reshapeWindow(window, w, h);
}

extern "C" void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  g_CurrentInstance->mouseHandler(window, xpos, ypos);
}

void Game::setupReshapeWindow() {
  ::g_CurrentInstance = this;
  ::glfwSetFramebufferSizeCallback(window, ::reshapeCallback);
}

void Game::setupMouseHandler() {
  ::g_CurrentInstance = this;
  ::glfwSetCursorPosCallback(window, ::mouseCallback);
}
