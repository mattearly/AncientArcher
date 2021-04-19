#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Version.h>
#include <iostream>
#include "setskybox.h"
#include "setfpsplayercontrols.h"
#include "setflashlight.h"
#include "setsoundeffects.h"
#include "setmusic.h"
#include "setdirlight.h"
#include "setmodels.h"

int main() {
  std::cout << "Running Version: " << AA::ENGINEVERSIONMAJOR << '.' << AA::ENGINEVERSIONMINOR << '.' << AA::ENGINEVERSIONPATCH << '\n';
  std::cout << "EXAMPLE PROJECT: " << "sample code for above version\n"
            << "Hotkeys:\n" 
            << "    -" << "WASD: Move\n"
            << "    -" << "Mouse: Look\n"
            << "    -" << "TAB:   Toggle Mouse Control\n"
            << "    -" << "F:  Toggle Flashlight\n"
            << "    -" << "1: Sound Effect Test (lazer sound)\n"
            << "    -" << "2: Sound Effect Test (enchant sound)\n"
            << "    -" << "F1: Toggle Sky Light\n"
            << "    -" << "P: Play Long Sound Test (Music)\n"
            << "    -" << "O: Stop Long Sound Test (Music)\n"
            << "    -" << "I: Pause Long Sound Test (Music)\n"
            << "    -" << "K: Resume Long Sound Test (Music)\n"
            << "    -" << "U: Unload Long Sound Test (Music)\n"
            << "    -" << "R: Reload Long Sound Test (Music)\n"
            << "    -" << "ScrollWheel: Vol up/down Long Sound Test (Music)\n"
            ;
  AA::InitEngine();
  static int ourcam = AA::AddCamera(AA::GetWindowWidth(), AA::GetWindowHeight());
  setfpsplayercontrols(ourcam);
  setskybox();
  setdirlight();
  setupflashlight(ourcam);
  setupsoundeffects();
  setmusic();
  setmodels(ourcam);

  AA::SetWindowClearColor(glm::vec3(0));
  return AA::Run();
}