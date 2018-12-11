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
  camera = new Camera();


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

  /* set up an area to store vertex data */
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  /* load textures */
  texBank.loadTexture("../AncientArcher/res/texture/00-pixelized_grass.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/01-pixelized_stone.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/02-pixelized_wood.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/03-pixelized_shrub.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/04-pixelized_portal.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/05-pixelized_darkstone.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/06-pixelized_water_droplet.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/07-pixelized_gravel.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/08-pixelized_water.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/09-pixelized_player.png", shader);
  texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);
  //texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);  // room for 5 more
  //texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);
  //texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);
  //texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);
  //texBank.loadTexture("../AncientArcher/res/texture/10-pixelized_mud.png", shader);

  /* update projection matrix  -  if changes, this needs to be moved to main loop or updated specifically when appropriate */
  glm::mat4 projection = glm::perspective(glm::radians(camera->FoV), (float)window_width / (float)window_height, 0.1f, 200.0f);
  shader->setMat4("projection", projection);

  //glBindBuffer(GL_ARRAY_BUFFER, 0);   //unbind VBO
  //glBindVertexArray(0);  //unbind VAO

  // only render the objects not line of sight blocked by other objects 
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
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
