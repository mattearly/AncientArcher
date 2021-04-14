#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Version.h>
#include <iostream>
#include "setskybox.h"
#include "setfpsplayercontrols.h"
#include "setflashlight.h"
#include "setsoundeffects.h"
#include "setmusic.h"
int main()
{
  std::cout << "Running Version: " << AA::ENGINEVERSIONMAJOR << '.' << AA::ENGINEVERSIONMINOR << '.' << AA::ENGINEVERSIONPATCH << '\n';
  AA::InitEngine();
  AA::SetWindowClearColor(glm::vec3(0));
  static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());
  setfpsplayercontrols(ourcam);
  setskybox();
  setupflashlight(ourcam);
  setupsoundeffects();
  setmusic();
 
  static int peasant_man = AA::AddProp("..\\ExampleProject\\res\\peasant_man.dae", ourcam, true);
  //static int test_zone = AA::AddProp("E:\\storage\\3d Models\\grass_plain.dae", ourcam, true);
  const float dist_from_us = 90.f;
  const float dist_down = -1.2f;

  AA::SetPropTranslation(peasant_man, glm::vec3(0, dist_down, -dist_from_us));
  AA::SetPropScale(peasant_man, glm::vec3(.1f));

  //AA::GetGameObject(test_zone).SetTranslation(glm::vec3(0, dist_down, 0));
  AA::SetDirectionalLight(glm::vec3(1.f), glm::vec3(.4f), glm::vec3(.4f), glm::vec3(1.f));
  AA::AddToDeltaUpdate([](float dt) {
    static float current_rotation = 0.f;
    current_rotation += dt * 5;
    current_rotation += dt * 2;
    AA::SetPropRotationY(peasant_man, current_rotation);
    }
  );
  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
    // turn directional light off
    if (kb.leftShift && kb.r) {
      AA::RemoveDirectionalLight();
      std::cout << "dir light turned off\n";
      return true;
    }
    // turn directional light on
    if (kb.r) {
      AA::SetDirectionalLight(glm::vec3(-.15f), glm::vec3(.2f), glm::vec3(.4f), glm::vec3(.3f));
      std::cout << "dir light turned on\n";
      return true;
    }
    return false;
    }
  );
  return AA::Run();
}