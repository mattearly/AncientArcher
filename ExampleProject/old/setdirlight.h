#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>
#ifdef _DEBUG
#include <iostream>
#endif

bool is_dir_light_setup = false;
bool dir_light_on = true;
AA::vec3 dir_light_direction = AA::vec3(.25f);
AA::vec3 dir_light_ambient = AA::vec3(.15f);
AA::vec3 dir_light_diff = AA::vec3(.15f);
AA::vec3 dir_light_spec = AA::vec3(.01f);

void setdirlight() {

  if (is_dir_light_setup)
    return;

  AA::SetDirectionalLight(dir_light_direction, dir_light_ambient, dir_light_diff, dir_light_spec);

  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) {
    if (kb.f1) {
      if (dir_light_on) {
        AA::RemoveDirectionalLight();
#ifdef _DEBUG
        std::cout << "dir light turned off\n";
#endif
        dir_light_on = false;
        return true;
      } else {
        AA::SetDirectionalLight(dir_light_direction, dir_light_ambient, dir_light_diff, dir_light_spec);
#ifdef _DEBUG
        std::cout << "dir light turned on\n";
#endif
        dir_light_on = true;
        return true;
      }
    }
    return false;
  }
  );

  is_dir_light_setup = true;
}

