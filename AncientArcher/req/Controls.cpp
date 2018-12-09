#include "Controls.h"
#include <glm/glm.hpp>
#include "Constraints.h"
#include "../src/Player.h"
#include "Sound.h"

Controls::Controls() {
  firstMouse = true;
  mouseSensitivity = 0.08f;
}

Controls::~Controls() {}

void Controls::mouseMovement(double xpos, double ypos, Camera *cam) {

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

  cam->Yaw += xoffset;
  cam->Pitch += yoffset;

  if (cam->Pitch > 89.0f) {
    cam->Pitch = 89.0f;
  } else if (cam->Pitch < -89.0f) {
    cam->Pitch = -89.0f;
  }

  cam->updateCameraVectors();

}

void Controls::keyboardInput(GLFWwindow * window, Camera *cam, float dtime) {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.boost = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.back = false;
      movedir.forward = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.forward = false;
      movedir.back = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.right = false;
      movedir.left = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    if (movedir.onGround) {
      movedir.left = false;
      movedir.right = true;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (movedir.onGround && movedir.canJumpAgain) {  //can jump again is to make the spacebar spam by holding it down not work
      movedir.jumped = true;
      movedir.canJumpAgain = false;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
    movedir.forward = false;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
    movedir.back = false;
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
    movedir.left = false;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
    movedir.right = false;
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
    movedir.boost = false;
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
    movedir.canJumpAgain = true;
  }

  if (movedir.back || movedir.forward || movedir.left || movedir.right) {

    float velocity = ((mainPlayer::LegPower / 10.0f) + 10.0f) * dtime;

    if (movedir.boost && !movedir.back) {
      velocity += velocity;   //faster move forward while holding shift
    }

    if (movedir.back || movedir.forward) {  // locks moving foward and backwards to the x and z axii, not that you can use the cam->Front instead of movefront to do a fly type thing
      glm::vec3 moveFront = { cam->Front.x, 0.0f, cam->Front.z };
      if (movedir.forward) cam->Position += moveFront * velocity;
      if (movedir.back) cam->Position -= moveFront * velocity;
    }

    if (movedir.right) cam->Position += cam->Right * velocity;
    if (movedir.left) cam->Position -= cam->Right * velocity;

    /* clamp to level - x */
    if (cam->Position.x > world_width) {
      if (movedir.forward) cam->Position.x -= cam->Front.x * velocity;
      if (movedir.back)    cam->Position.x += cam->Front.x * velocity;
      if (movedir.right)   cam->Position.x -= cam->Right.x * velocity;
      if (movedir.left)    cam->Position.x += cam->Right.x * velocity;
    } else if (cam->Position.x < 0.0f) {
      if (movedir.forward) cam->Position.x -= cam->Front.x * velocity;
      if (movedir.back)    cam->Position.x += cam->Front.x * velocity;
      if (movedir.right)   cam->Position.x -= cam->Right.x * velocity;
      if (movedir.left)    cam->Position.x += cam->Right.x * velocity;
    } 

    /* clamp to level - z */
    if (cam->Position.z > world_width) {
      if (movedir.forward) cam->Position.z -= cam->Front.z * velocity;
      if (movedir.back)    cam->Position.z += cam->Front.z * velocity;
      if (movedir.right)   cam->Position.z -= cam->Right.z * velocity;
      if (movedir.left)    cam->Position.z += cam->Right.z * velocity;
    } else if (cam->Position.z < 0.0f) {
      if (movedir.forward) cam->Position.z -= cam->Front.z * velocity;
      if (movedir.back)    cam->Position.z += cam->Front.z * velocity;
      if (movedir.right)   cam->Position.z -= cam->Right.z * velocity;
      if (movedir.left)    cam->Position.z += cam->Right.z * velocity;
    }

  }

  /* Jump System */
  if (movedir.jumped) {   // Jump Start
    movedir.onGround = false;
    movedir.jumped = false;
    playgruntsound();
  } else if (!movedir.onGround && !movedir.falling) {  // Jump Rising
    cam->Position.y += cam->WorldUp.y * ((mainPlayer::LegPower / 10.0f) + 4.0f) * dtime;  // RISING SPEED CALC: jump speed based on LegPower Player Stat
    if (cam->Position.y > (mainPlayer::LegPower / 10.0f) + 0.8f + camstart[1]) {  // MAX HEIGHT CALC: jump height based on LegPower Player Stat
      movedir.falling = true;
      //todo: added gravity to falling y = 1/2at^2 + vt  
    }
  } else if (movedir.falling && !movedir.onGround) {   // currently going down
    cam->Position.y -= cam->WorldUp.y * 5.0f * dtime;  // GRAVITY PULL DOWN CALC: static value, todo: make dynamic based on falling time
    if (cam->Position.y <= camstart[1]) {
      movedir.onGround = true;
      movedir.falling = false;
      playlandingsound();
    }
  } 

  if (movedir.onGround) {
    cam->Position.y = camstart[1];
  }

}