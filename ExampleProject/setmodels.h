#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>
#include <AncientArcher/Utility/SceneCheck.h>
#include <AncientArcher/ShaderType.h>
bool is_models_set = false;
void setmodels(int cam) {
  if (is_models_set)
    return;
  is_models_set = true;
  int inherited_cam = cam;
  static int peasant_man = AA::AddProp("..\\ExampleProject\\res\\peasant_man.dae", inherited_cam, AA::SHADERTYPE::LIT);

  // mesh demos
  //const float dist_from_us = 200.f;
  //const float dist_down = 10.f;

  //AA::printSceneHeaderInfo(AA::extractSceneInfo("..\\ExampleProject\\res\\peasant_man.dae"));
  //AA::SetPropTranslation(peasant_man, AA::vec3(0, -dist_down, -dist_from_us));
  //AA::AddToDeltaUpdate([](float dt) {
  //  static float current_rotation = 0.f;
  //  current_rotation += dt * 120;
  //  AA::SetPropRotationY(peasant_man, current_rotation);
  //}
  //);
}