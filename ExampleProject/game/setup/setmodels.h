#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Utility/SceneCheck.h>
#include <glm/glm.hpp>
#include <imgui.h>
extern AA::AncientArcher instance;

bool is_models_set = false;
void setmodels() {
  if (is_models_set)
    return;
  is_models_set = true;

  /// <summary>
  /// Place a walking model
  /// </summary>
  {
    static glm::vec3 base_loc = glm::vec3(160, -99, -1000);
    static float base_scale = 1;
    static glm::vec3 base_rot = { 0, 0, 0 };
    static const char* model_path = "res/Walking.dae";
    static unsigned int model_id = instance.AddAnimProp_testing(model_path, true, base_loc);

    instance.AddToImGuiUpdate([]() {
      ImGui::Begin(model_path);
      ImGui::SliderFloat3("loc", &base_loc[0], -1000.f, 1000.f);
      ImGui::SliderFloat3("rot", &base_rot[0], -3.14f, 3.14f);
      ImGui::SliderFloat("sca", &base_scale, 1.f, 200.f);
      ImGui::End();

      instance.MoveAnimProp(model_id, base_loc);
      instance.ScaleAnimProp(model_id, glm::vec3(base_scale));
      instance.RotateAnimProp(model_id, base_rot);
    });

    instance.AddToUpdate([](float dt){
      glm::vec3 new_loc = base_loc + glm::vec3(0,0,+dt*110);
      instance.MoveAnimProp(model_id, new_loc);
      base_loc = new_loc;
      if (base_loc.z > 2000.f) {
        base_loc.z = -1000;
      }
    });
  }

  /// <summary>
  /// Place a walking model
  /// </summary>
  {
    static glm::vec3 base_loc = glm::vec3(160, -99, -1000);
    static float base_scale = 1;
    static glm::vec3 base_rot = { 0, 0, 0 };
    static const char* model_path = "res/Zombie Punching.fbx";
    static unsigned int model_id = instance.AddAnimProp_testing(model_path, true, base_loc);

    instance.AddToImGuiUpdate([]() {
      ImGui::Begin(model_path);
      ImGui::SliderFloat3("loc", &base_loc[0], -1000.f, 1000.f);
      ImGui::SliderFloat3("rot", &base_rot[0], -3.14f, 3.14f);
      ImGui::SliderFloat("sca", &base_scale, 1.f, 200.f);
      ImGui::End();

      instance.MoveAnimProp(model_id, base_loc);
      instance.ScaleAnimProp(model_id, glm::vec3(base_scale));
      instance.RotateAnimProp(model_id, base_rot);
    });
  }

  /// <summary>
  /// Places static ground, re-using a plane 
  /// </summary>
  /// 
  {
    const int half_repeat = 4;  // the number of tiles generated around the point 0,0 is double this number
    const int plane_stride = 400;
    for (int i = -plane_stride * half_repeat; i < plane_stride * half_repeat * 2; i += plane_stride) {
      for (int j = -plane_stride * half_repeat; j < plane_stride * half_repeat * 2; j += plane_stride) {
        int large_green_plane = instance.AddProp("res/large_green_plane.obj", true, glm::vec3(i, -99, j));
      }
    }
  }


  /// <summary>
  /// Places static mesh that rotates
  /// </summary>
  static unsigned int peasant_man = instance.AddProp("res/peasant_man.dae", true, glm::vec3(0, -99, -400));
  instance.AddToUpdate([](float dt){
    static float y_rad = 0;
    y_rad += dt * 3.14f;
    instance.RotateProp(peasant_man, glm::vec3(0, y_rad, 0));
  });

}




