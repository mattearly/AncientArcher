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
}

void InitEngineAndGame() {
#if _DEBUG 
  printf("AA init ver: %i.%i.%i\n", AA::ENGINEVERSIONMAJOR, AA::ENGINEVERSIONMINOR, AA::ENGINEVERSIONPATCH);
#endif
  AA::Init();
  my_game.Setup();
  AA::AddToDeltaUpdate([](float dt) { engine_tick(dt); });
}

int main(int argc, char** argv) {
  InitEngineAndGame();
  return AA::Run();
}
