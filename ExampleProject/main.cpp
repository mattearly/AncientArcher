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
            << "    -" << "WASD:  Move\n"
            << "    -" << "Mouse: Look\n"
            << "    -" << "TAB:   Toggle Mouse Control (Inventory GUI)\n"
            << "    -" << "F:     Toggle Flashlight\n"
            << "    -" << "1:     Sound Effect (lazer sound)\n"
            << "    -" << "2:     Sound Effect (enchant sound)\n"
            << "    -" << "F1:    Toggle Sky Light\n"
            << "    -" << "P:     Play Long Sound (Music)\n"
            << "    -" << "O:     Stop Long Sound (Music)\n"
            << "    -" << "I:     Pause Long Sound (Music)\n"
            << "    -" << "K:     Resume Long Sound (Music)\n"
            << "    -" << "U:     Unload Long Sound (Music)\n"
            << "    -" << "R:     Reload Long Sound (Music)\n"
            << "    -" << "Wheel: Vol Up/Down (Music)\n"
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