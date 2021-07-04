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

//#ifdef _DEBUG
//  AA::printSceneHeaderInfo(AA::extractSceneInfo("..\\ExampleProject\\res\\peasant_man.dae"));
//#endif
  //// spinning peasant man demo
  //static int peasant_man = AA::AddProp("..\\ExampleProject\\res\\peasant_man.dae", inherited_cam, AA::SHADERTYPE::LIT);
  const float dist_from_us = 200.f;
  const float dist_down = 10.f;
  //AA::SetPropTranslation(peasant_man, AA::vec3(0, -dist_down, -dist_from_us));
  //AA::AddToDeltaUpdate([](float dt) {
  //  static float current_rotation = 0.f;
  //  current_rotation += dt * 120;
  //  AA::SetPropRotationY(peasant_man, current_rotation);
  //}
  //);

  // spinning peasant man demo

#ifdef _DEBUG
  AA::printSceneHeaderInfo(AA::extractSceneInfo("..\\ExampleProject\\res\\walking_nightshade.fbx"));
#endif


  static int warlock_chick = AA::AddProp("..\\ExampleProject\\res\\walking_nightshade.fbx", inherited_cam, AA::SHADERTYPE::LIT, true);
  AA::SetPropTranslation(warlock_chick, AA::vec3(0, -dist_down, -dist_from_us));
  //AA::AddToDeltaUpdate([](float dt) {
  //  static float current_rotation = 0.f;
  //  current_rotation += dt * 120;
  //  AA::SetPropRotationY(warlock_chick, current_rotation);
  //}
  //);

  // plane test
  //static int plane_id = AA::AddPlane();

  static int plane
  //= AA::AddProp("E:\\AssetPack\\plane.obj", inherited_cam, AA::SHADERTYPE::LIT);
  = AA::AddPlane();

  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb)->bool{ 
    static bool plane_live = true;
    if (kb.p) {
      if (plane_live)
      {
        AA::RemovePlane(plane);
        plane_live = false;
      }
      else
      {
        plane = AA::AddPlane();
        plane_live = true;
      }

      return true;
    }
    return false;
  });
}