#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>
bool is_models_set = false;
void setmodels(int cam)
{
  if (is_models_set)
    return;

  int inherited_cam = cam;
  // peasant man spinning demo
  const float dist_from_us = 35.f;
  const float dist_down = -6.7f;
  static int peasant_man = AA::AddProp("..\\ExampleProject\\res\\peasant_man.dae", inherited_cam, AA::SHADERTYPE::LIT);
  AA::SetPropTranslation(peasant_man, glm::vec3(0, dist_down, -dist_from_us));
  AA::SetPropScale(peasant_man, glm::vec3(.1f));
  AA::AddToDeltaUpdate([](float dt) {
    static float current_rotation = 0.f;
    current_rotation += dt * 2;
    AA::SetPropRotationY(peasant_man, current_rotation);
  }
  );

  is_models_set = true;
}