#include <Controls.h>
#include <Display.h>
#include <Camera.h>

extern Display g_display;   // from game.cpp (game)
extern Camera g_camera;     // from game.cpp (game)

/**
 *  Default Constructor.
 */
Controls::Controls() {
  firstMouse = true;
  mouseSensitivity = 0.09f;
  lastX = 0.f;
  lastY = 0.f;
}

/**
 *  First Person Mouse moves Controls. Cursor visiblity is best hidden (see display class)
 *  when using this method.
 */
void Controls::FPPmouseMovement(float xpos, float ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  g_camera.Yaw += xoffset;
  g_camera.Pitch += yoffset;

  if (g_camera.Pitch > 89.0f)
  {
    g_camera.Pitch = 89.0f;
  }
  else if (g_camera.Pitch < -89.0f)
  {
    g_camera.Pitch = -89.0f;
  }

  g_camera.updateCameraVectors();
}

/**
 *  In Development.
 */
void Controls::SSmouseMovement(float xpos, float ypos)
{
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  // TODO HANDLE NEW MOUSE POSITION 
  // OR WAIT FOR CLICK TO HANDLE NEW MOUSE POSITION in sideScrollPlayerKeyboardInput()
}


/**
 *  fpp keyboard controls
 */
void Controls::fppKeyboardIn(FirstPersonPlayer* fpp)
{
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    fpp->moves.interacting = true;
  }


  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    fpp->moves.usingTool = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(g_display.window, true);
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    if (fpp->moves.onGround) {
      fpp->moves.boost = true;
    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_PRESS) {
    if (fpp->moves.onGround) {
      fpp->moves.back = false;
      fpp->moves.forward = true;
    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_PRESS) {
    if (fpp->moves.onGround) {
      fpp->moves.forward = false;
      fpp->moves.back = true;

    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_PRESS) {
    if (fpp->moves.onGround) {
      fpp->moves.right = false;
      fpp->moves.left = true;

    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_PRESS) {
    if (fpp->moves.onGround) {
      fpp->moves.left = false;
      fpp->moves.right = true;
    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_M) == GLFW_PRESS) {
    //toggleAmbientWindyNight();
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (fpp->moves.onGround && fpp->moves.canJumpAgain) {  //can jump again is to make the spacebar spam by holding it down not work
      fpp->moves.onGround = false;
      fpp->moves.canJumpAgain = false;
    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_0) == GLFW_PRESS) {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_PRESS) {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_PRESS) {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_RELEASE) {
    fpp->moves.forward = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_RELEASE) {
    fpp->moves.back = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_RELEASE) {
    fpp->moves.left = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_RELEASE) {
    fpp->moves.right = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    fpp->moves.boost = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    fpp->moves.canJumpAgain = true;
  }
}

/**
 *  Forwards Side Scroll Player Controls
 */
void Controls::sideScrollPlayerKeyboardInput(SideScrollPlayer* ssp)
{
  static unsigned i = 0;
  i = 0;
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    ssp->spawnSword();
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(g_display.window, true);  // closes app
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_PRESS)
  {
    ssp->moves.forward = false;
    ssp->moves.backward = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_PRESS)
  {
    ssp->moves.backward = false;
    ssp->moves.forward = true;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_M) == GLFW_PRESS)
  {
    //toggleAmbientWindyNight();
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    unsigned int count = 1;
    if (ssp->isAttacking()) count++;
    for (i = 0; i < count; ++i)
    {
      (ssp->getEntity() + i)->moveTo(glm::vec3((ssp->getEntity() + i)->gameItem.loc.x, 4.2f, 0.f));  // needs delta time just testing
    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_0) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_RELEASE)
  {
    ssp->moves.backward = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_RELEASE)
  {
    ssp->moves.forward = false;
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
    unsigned int count = 1;
    if (ssp->isAttacking()) count++;
    for (i = 0; i < count; ++i)
    {
      (ssp->getEntity() + i)->moveTo(glm::vec3((ssp->getEntity() + i)->gameItem.loc.x, 2.5f, 0.f));  // needs delta time just testing
    }
  }
}

/**
 *  Forwards Controls for an entity.
 */
void Controls::entityKeyboardInput(Entity* entity)
{
  // todo: need bools to go with, this just keep repeating if held

  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    // left click
  /*  playbowsound();*/
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(g_display.window, true);  // closes app
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_PRESS)
  {
    entity->moveBy(glm::vec3(-0.1f, 0.0f, 0.0f));  // needs delta time just testing
    g_camera.Position.x -= 0.1f;   // hack to keep the cam in place with the player
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_PRESS)
  {
    entity->moveBy(glm::vec3(0.1f, 0.0f, 0.0f));  // needs delta time just testing
    g_camera.Position.x += 0.1f;   // hack to keep the cam in place with the player

  }

  if (glfwGetKey(g_display.window, GLFW_KEY_M) == GLFW_PRESS)
  {
    //toggleAmbientWindyNight();
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    entity->moveTo(glm::vec3(entity->gameItem.loc.x, 3.2f, 0.f));  // needs delta time just testing
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_0) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
    entity->moveTo(glm::vec3(entity->gameItem.loc.x, 2.2f, 0.f));  // needs delta time just testing
  }

}

/**
 *  Forwards Controls for more than one entity.
 */
void Controls::entitiesKeyboardInput(Entity* entity, unsigned int numEntities)
{

  // todo: need bools to go with, this just keep repeating if held
  static unsigned int i = 0;
  i = 0;
  if (glfwGetMouseButton(g_display.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(g_display.window, true);  // closes app
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_PRESS)
  {
    for (i = 0; i < numEntities; ++i)
    {
      (entity + i)->moveBy(glm::vec3(-0.1f, 0.0f, 0.0f));  // needs delta time just testing
    }
    g_camera.Position.x -= 0.1f;   // hack to keep the cam in place with the player
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_PRESS)
  {
    for (i = 0; i < numEntities; ++i)
    {
      (entity + i)->moveBy(glm::vec3(0.1f, 0.0f, 0.0f));  // needs delta time just testing
    }
    g_camera.Position.x += 0.1f;   // hack to keep the cam in place with the player

  }

  if (glfwGetKey(g_display.window, GLFW_KEY_M) == GLFW_PRESS)
  {
    //toggleAmbientWindyNight();
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    for (i = 0; i < numEntities; ++i)
    {
      (entity + i)->moveTo(glm::vec3((entity + i)->gameItem.loc.x, 4.3f, 0.f));  // needs delta time just testing
    }
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_0) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_1) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_2) == GLFW_PRESS)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_W) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_S) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_A) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_D) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
  }

  if (glfwGetKey(g_display.window, GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
    for (i = 0; i < numEntities; ++i)
    {
      (entity + i)->moveTo(glm::vec3((entity + i)->gameItem.loc.x, 5.f, 0.f));  // needs delta time just testing
    }
  }

}


