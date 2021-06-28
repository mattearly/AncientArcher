#pragma once
#include <AncientArcher/AncientArcher.h>
#include <glm/glm.hpp>
struct FlashLight {
  bool      isOn = false;
  int       id = -1;
  glm::vec3 position = glm::vec3(0);
  glm::vec3 direction = glm::vec3(0, -1, 0);
  float     inner_radius = glm::cos(glm::radians(12.05f));
  float     outer_radius = glm::cos(glm::radians(34.05f));
  float     constant = 1.f;
  float     linear = 0.007f;
  float     quad = 0.0002f;
  glm::vec3 ambient = glm::vec3(1.23f);
  glm::vec3 diffuse = glm::vec3(3.57f);
  glm::vec3 specular = glm::vec3(1.0f);
  void turn_on() {
    if (isOn) return;
    id = AA::AddSpotLight(position, direction, inner_radius, outer_radius, constant, linear, quad, ambient, diffuse, specular);
    isOn = true;
  };
  void turn_off() {
    if (!isOn) return;
    AA::RemoveSpotLight(id);
    id = -1;
    isOn = false;
  };
  void lock_in_new_params() {
    AA::ChangeSpotLight(id, position, direction, inner_radius, outer_radius, constant, linear, quad, ambient, diffuse, specular);
  }
  void lock_in_movement() {
    AA::MoveSpotLight(id, position, direction);
  }
};
