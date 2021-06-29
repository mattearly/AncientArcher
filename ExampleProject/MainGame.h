#pragma once
#include <AncientArcher/AncientArcher.h>
#include "MainCharacter.h"
#include "old/setskybox.h"
#include "old/setmodels.h"
#include "old/setdirlight.h"

MainCharacter my_character;
extern int character_cam;

struct MainGame {

  void Setup() {
    my_character.Setup();
    AA::SetTimedOutKeyHandlingLength(.23f); 
    setmodels(character_cam);
    setskybox();   // sets to the first cam (engine)
    setdirlight();  // sets the world light
  };

  void Update() {
    my_character.Update();
  };

  void Update(float dt) {
    my_character.Update(dt);
  };

  void Teardown() {
    my_character.Teardown();
  };

};