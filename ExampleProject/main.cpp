#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Version.h>
#include <AncientArcher/Utility/Files.h>
#include <iostream>
#include "setskybox.h"
#include "setfpsplayercontrols.h"
#include "setflashlight.h"
#include "setsoundeffects.h"
#include "setmusic.h"
#include "setdirlight.h"
#include "setmodels.h"
#include "setgui.h"
#include "setupengine.h"
#include "setplayerlightradius.h"

int main(int argc, char** argv) {
  std::cout << "Running: " << AA::Files::extract_prog_name(argv[0]) << '\n';
  setupengine();
  static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());
  AA::SetTimedOutKeyHandlingLength(.23f);

  setfpsplayercontrols(ourcam);
  setupflashlight(ourcam);
  setplayerlightradius(ourcam);
  setgui(ourcam);
  setmodels(ourcam);
  setskybox();
  setdirlight();
  setupsoundeffects();
  setmusic();
  AA::AddToKeyHandling([](AA::KeyboardInput& kb){
    if (kb.leftShift && kb.o) {
      static bool perspective = true;
      if (!perspective) {
        AA::SetCamToPerspective(ourcam);
      } else {
        AA::SetCamToOrtho(ourcam);
      }
      perspective = !perspective;
      }
  });

  //setanimmodels(ourcam);
  AA::SetWindowClearColor(glm::vec3(0));
  return AA::Run();
}