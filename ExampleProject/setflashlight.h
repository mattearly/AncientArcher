#pragma once
#include <AncientArcher/AncientArcher.h>
#include <iostream>

bool      isFlashlightSetup = false;
bool      flashlight_on = false;
int       flashlight = -1;
float     fl_inner_radius = glm::cos(glm::radians(2.05f));
float     fl_outer_radius = glm::cos(glm::radians(17.05f));
float     fl_constant = 1.f;
float     fl_linear = 0.039f;
float     fl_quad = 0.0044f;
glm::vec3 fl_ambient = glm::vec3(1.23f);
glm::vec3 fl_diffuse = glm::vec3(3.57f);
glm::vec3 fl_specular = glm::vec3(1.0f);
void setupflashlight(int ourcam)
{
  if (isFlashlightSetup)
    return;

  isFlashlightSetup = true;
  static int cam = ourcam;

  // toggle flashlight
  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
    if (kb.f)
    {
      if (flashlight_on)
      {
        AA::RemoveSpotLight(flashlight);
        flashlight_on = false;
        std::cout << "flashlight turned off\n";
      }
      else
      {
        flashlight = AA::AddSpotLight(
          AA::GetCamPosition(cam),
          AA::GetCamFront(cam),
          fl_inner_radius,
          fl_outer_radius,
          fl_constant,
          fl_linear,
          fl_quad,
          fl_ambient,
          fl_diffuse,
          fl_specular
        );
        flashlight_on = true;
        std::cout << "flashlight turned on\n";
      }
      return true;
    }
    return false;
    }
  );

  //stick flashlight to camera
  AA::AddToUpdate([]() {
    if (flashlight_on)
      AA::MoveSpotLight(flashlight,
        AA::GetCamPosition(cam) + glm::vec3(0.f, -.5f, 0.f),
        AA::GetCamFront(cam));
    });
}