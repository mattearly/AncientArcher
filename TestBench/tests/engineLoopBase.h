#pragma once
#include <iostream>
#include "../world/AAWorld.h"

int testEngineLoopBase()
{
  // Instantiate world with a title.
  AAWorld world("Base World Loop Test(close to quit)");

  // Add to loop functions.
  world.addToOnBegin([]()noexcept {printf("World Started. timestamp: %s \n", __TIMESTAMP__); });
  world.addToDeltaUpdate([](float step)noexcept {static float timePassed = 0; timePassed += step; const float newcolor = abs(sin(timePassed)); AADisplay::getInstance()->setWindowClearColor(glm::vec3(newcolor, newcolor, newcolor)); });
  world.addToDeltaUpdate([](float step) {std::cout << ".      process delta update: " << step << '\n'; });
  world.addToOnRender([]() {std::cout << "..     process render\n"; });
  world.addToTimedOutKeyHandling([](AAKeyBoardInput& keys)noexcept {printf("...    %fms timedout keyboard/mouse\n", 90.f); return false;  });
  world.addToKeyHandling([](AAKeyBoardInput& keys) {std::cout << "....   process keyboard/mouse \n";  });
  world.addToScrollHandling([](AAScrollInput& scroll) {std::cout << ".....  process mouse scroll: X:" << scroll.xOffset << " Y:" << scroll.yOffset << '\n'; });
  world.addToMouseHandling([](AAMouseInput& pointer) {std::cout << "...... process mouse pointer: X:" << pointer.xOffset << " Y:" << pointer.yOffset << '\n';  });
  world.addToUpdate([]() {std::cout << ".......process update\n"; });
  return world.run();
}