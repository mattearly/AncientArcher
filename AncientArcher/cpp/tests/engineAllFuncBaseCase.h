#pragma once
#include "../engine/AAEngine.h"
#include <iostream>
#include "dry/addGodMovement.h"

void testEngineFuncsBaseCase()
{
  AAEngine engine;

  auto begincout = []() {

    AADisplay::getInstance()->setWindowTitle("all functions loop - close window to continue");  // test changing window title
    AADisplay::getInstance()->setWindowClearColor(glm::vec3(0));        // test changing window clear color
    std::cout << "->One Time Begin Run\n"; 
  };
  engine.addToOnBegin(begincout);

  auto deltaChangeClearColor = [](float step) {
    static float timePassed = 0;
    timePassed += step;
    float newcolor = abs(sin(timePassed));
    AADisplay::getInstance()->setWindowClearColor(
      glm::vec3(
        newcolor,
        newcolor,
        newcolor
      )
    );        // test changing window clear color to random values
  };
  engine.addToDeltaUpdate(deltaChangeClearColor);

  auto deltacout = [](float step) {std::cout << ".    process delta update: " << step << '\n'; };
  engine.addToDeltaUpdate(deltacout);

  auto rendercout = []() {std::cout << "..   process render\n"; };
  engine.addToOnRender(rendercout);

  auto customhandleinput = [](AAKeyBoardInput& keys) {
    std::cout << "...  process keyboard/mouse \n";
  };
  engine.addToKeyHandling(customhandleinput);

  addGodMovement(engine);

  auto updatecout = []() {std::cout << ".... process update\n"; };
  engine.addToUpdate(updatecout);

  switch (engine.run())
  {
  case -1:
    std::cout << "Keys not set.\n";
    break;
  case -2:
    std::cout << "Mouse not set.\n";
    break;
  case -3:
    std::cout << "Scroll not set.\n";
    break;
  default:
    break;
  }
}