#include <Camera.h>
#include <Global.h>
#include <Display.h>
#include <Global.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>  

extern Display g_display;

Camera::Camera(const glm::vec3 startingPos, const float lookDir, const float pitch, const float fov)
{
  Position = startingPos;
  Yaw = lookDir;
  Pitch = pitch; 

  updateCameraVectors();  // set Front, Up, & Right

  FoV = fov;

}

void Camera::update(Shader* shader) {
  glm::mat4 view = getViewMatrix();
  shader->use();
  shader->setMat4("view", view);
}

// needs called to update the FoV and/or window_width window_height, and draw distances
// this is for the global texBankShader
void Camera::updateProjectionMatrix(Shader* shader) {
  glm::mat4 projection = glm::perspective(glm::radians(FoV), (float)g_display.window_width / (float)g_display.window_height, 0.01f, RENDER_DISTANCE);
  shader->use();
  shader->setMat4("projection", projection);
}

glm::mat4 Camera::getProjectionMatrix()
{
  glm::mat4 projection = glm::perspective(glm::radians(FoV), (float)g_display.window_width / (float)g_display.window_height, 0.01f, RENDER_DISTANCE);

  return projection;
}

void Camera::setPosition(glm::vec3 pos)
{
  Position = pos;
  //updateCameraVectors();  //update front, up, right 
}

void Camera::increaseYawAndPitch(float yawOff, float pitchOff)
{
  increaseYaw(yawOff);
  increasePitch(pitchOff);
  updateCameraVectors();
}

void Camera::increaseYaw(float offset)
{
  Yaw += offset;
}

void Camera::increasePitch(float offset)
{
  Pitch += offset;

  if (this->Pitch > 89.0f)
  {
    this->Pitch = 89.0f;
  }
  else if (this->Pitch < -89.0f)
  {
    this->Pitch = -89.0f;
  }
}

void Camera::setToOrtho(Shader* shader)
{
  //ortho(T const& left, T const& right, T const& bottom, T const& top, T const& zNear, T const& zFar)

  glm::mat4 projection = glm::ortho(-(float)g_display.window_width / 2.f, (float)g_display.window_width / 2.f, -(float)g_display.window_height / 2.f, (float)g_display.window_height / 2.f, -1.f, 1.f);
  shader->use();
  shader->setMat4("projection", projection);
}

void Camera::setToPerspective(Shader* shader, float fov)
{
  glm::mat4 projection = glm::perspective(glm::radians(FoV), (float)g_display.window_width / (float)g_display.window_height, 0.01f, RENDER_DISTANCE);
  shader->use();
  shader->setMat4("projection", projection);
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3* Camera::getPosition() {
  return &Position;
}

glm::vec3* Camera::getFront() {
  return &Front;
}

glm::vec3* Camera::getRight() {
  return &Right;
}

float Camera::getYaw() {
  return Yaw;
}

float Camera::getPitch() {
  return Pitch;
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WORLD_UP));
  Up = glm::normalize(glm::cross(Right, Front));
}
