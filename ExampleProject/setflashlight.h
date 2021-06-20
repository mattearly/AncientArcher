#pragma once
#include <AncientArcher/AncientArcher.h>
#include <iostream>

bool      isFlashlightSetup = false;
bool      flashlight_on = false;
int       flashlight_id = -1;
float     fl_inner_radius = glm::cos(glm::radians(12.05f));
float     fl_outer_radius = glm::cos(glm::radians(34.05f));
float     fl_constant = 1.f;
float     fl_linear = 0.007f;
float     fl_quad = 0.0002f;
glm::vec3 fl_ambient = glm::vec3(1.23f);
glm::vec3 fl_diffuse = glm::vec3(3.57f);
glm::vec3 fl_specular = glm::vec3(1.0f);

void setupflashlight(int ourcam) {
  if (isFlashlightSetup)
    return;

  isFlashlightSetup = true;
  static int cam = ourcam;

  flashlight_id = AA::AddSpotLight(
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

  std::cout << ", f = toggle flashlight";
  AA::SetTimedOutKeyHandlingLength(.1f);
  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
    if (kb.f) {
      if (flashlight_on) {
        AA::RemoveSpotLight(flashlight_id);
        flashlight_on = false;
#ifdef _DEBUG
        std::cout << "flashlight turned off\n";
#endif
      } else {
        flashlight_id = AA::AddSpotLight(
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
#ifdef _DEBUG
        std::cout << "flashlight turned on\n";
#endif
      }
      return true;
    }
    return false;
  }
  );

  //stick flashlight_id to camera
  AA::AddToUpdate([]() {
    if (flashlight_on)
      AA::MoveSpotLight(flashlight_id, AA::GetCamPosition(cam), AA::GetCamFront(cam));
  });
}