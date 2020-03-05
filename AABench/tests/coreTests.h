/*
  Test building a small world with different shaders.
  Test Collision.
  Test basic sound effects functionality.
  Test demo asssets.
  Test Assimp base model header loading.
*/
#pragma once
#include "dry/addGodMovement.h"
#include "dry/addSkybox.h"
#include "dry/addRandomWorld.h"
#include <string>

extern AAWorld mainWorld;

int testEngineLoopBase()
{
  mainWorld.setWindowTitle("demo loop base");
  // Add to loop functions.
  mainWorld.addToOnBegin([]()noexcept {printf("World Started. timestamp: %s \n", __TIMESTAMP__); });
  mainWorld.addToDeltaUpdate([](float step)noexcept {static float timePassed = 0; timePassed += step; const float newcolor = abs(sin(timePassed)); AADisplay::getInstance()->setWindowClearColor(glm::vec3(newcolor, newcolor, newcolor)); });
  mainWorld.addToDeltaUpdate([](float step) {std::cout << ".      process delta update: " << step << '\n'; });
  mainWorld.addToTimedOutKeyHandling([](AAKeyBoardInput& keys)noexcept {printf("...    %fms timedout keyboard/mouse\n", 90.f); return false;  });
  mainWorld.addToKeyHandling([](AAKeyBoardInput& keys) {std::cout << "....   process keyboard/mouse \n";  });
  mainWorld.addToScrollHandling([](AAScrollInput& scroll) {std::cout << ".....  process mouse scroll: X:" << scroll.xOffset << " Y:" << scroll.yOffset << '\n'; });
  mainWorld.addToMouseHandling([](AAMouseInput& pointer) {std::cout << "...... process mouse pointer: X:" << pointer.xOffset << " Y:" << pointer.yOffset << '\n';  });
  mainWorld.addToUpdate([]() {std::cout << ".......process update\n"; });
  return mainWorld.runMainLoop();
}

int testEngineDemoWorld()
{
  mainWorld.setWindowTitle("Test Engine Demo World");
  const int cameraId = setGodCamWithMovement();
  addRandomWorld(cameraId);
  addCubeSkybox();
  return mainWorld.runMainLoop();
}

void runAllCoreTests(int& result)
{
  result = testEngineLoopBase();
  result = testEngineDemoWorld();
}