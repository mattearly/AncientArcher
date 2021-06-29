#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Version.h>
#include <AncientArcher/Utility/Files.h>
#include "BattleStats.h"
#include "MainCharacter.h"
#include "MainGame.h"
#if _DEBUG
#include <iostream>
#endif

MainGame my_game;

void engine_tick(float dt) {
  my_game.Update();
  my_game.Update(dt);
  //std::cout << "tick: " << dt << '\n';
}

void InitEngineAndGame() {
#if _DEBUG 
  printf("AA init ver: %i.%i.%i\n", AA::ENGINEVERSIONMAJOR, AA::ENGINEVERSIONMINOR, AA::ENGINEVERSIONPATCH);
#endif
  AA::Init();
  my_game.Setup();

  AA::AddToTimedOutKeyHandling([](AA::KeyboardInput& kb) -> bool {
    if (kb.enter && (kb.leftAlt || kb.rightAlt)) {
#if _DEBUG
      std::cout << "toggle fullscreen\n";
#endif
      AA::ToggleFullscreen();
      return true;
    }
  });

  AA::AddToDeltaUpdate([](float dt) { engine_tick(dt); });

}

int main(int argc, char** argv) {
  InitEngineAndGame();
  return AA::Run();
}
