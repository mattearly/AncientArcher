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
  _position = startingPos;
  _yaw = lookDir;
  _pitch = pitch;

  _updateCameraVectors();  // set Front, Up, & Right

  _fov = fov;

}

void Camera::update(Shader* shader)
{
  shader->use();
  shader->setMat4("view", getViewMatrix());
}

// needs called to update the FoV and/or window_width window_height, and draw distances
// this is for the global texBankShader
void Camera::updateProjectionMatrix(Shader* shader) {
  glm::mat4 projection = glm::perspective(glm::radians(_fov), (float)g_display.window_width / (float)g_display.window_height, 0.01f, RENDER_DISTANCE);
  shader->use();
  shader->setMat4("projection", projection);
}

glm::mat4 Camera::getProjectionMatrix()
{
  glm::mat4 projection = glm::perspective(glm::radians(_fov), (float)g_display.window_width / (float)g_display.window_height, 0.01f, RENDER_DISTANCE);

  return projection;
}

void Camera::setPosition(glm::vec3 pos)
{
  _position = pos;
  _updateCameraVectors();  //update front, up, right   --- todo: should we do this here?
}

void Camera::increaseYawAndPitch(float yawOff, float pitchOff)
{
  _increaseYaw(yawOff);
  _increasePitch(pitchOff);
  _updateCameraVectors();
}

void Camera::increasePosition(glm::vec3 amount)
{
  _position += amount;
  _updateCameraVectors();  //update front, up, right   --- todo: should we do this here?

}

void Camera::_increaseYaw(float offset)
{
  _yaw += offset;
}

void Camera::_increasePitch(float offset)
{
  _pitch += offset;

  if (this->_pitch > 89.0f)
  {
    this->_pitch = 89.0f;
  }
  else if (this->_pitch < -89.0f)
  {
    this->_pitch = -89.0f;
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
  glm::mat4 projection = glm::perspective(glm::radians(_fov), (float)g_display.window_width / (float)g_display.window_height, 0.01f, RENDER_DISTANCE);
  shader->use();
  shader->setMat4("projection", projection);
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(_position, _position + _front, _up);
}

glm::vec3* Camera::getPosition() {
  return &_position;
}

glm::vec3* Camera::getFront() {
  return &_front;
}

glm::vec3* Camera::getRight() {
  return &_right;
}

float Camera::getYaw() {
  return _yaw;
}

float Camera::getPitch() {
  return _pitch;
}

void Camera::_updateCameraVectors() {
  glm::vec3 front;
  front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
  front.y = sin(glm::radians(_pitch));
  front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

  _front = glm::normalize(front);
  _right = glm::normalize(glm::cross(_front, WORLD_UP));
  _up = glm::normalize(glm::cross(_right, _front));
}
