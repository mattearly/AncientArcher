#pragma once
#include <iostream>

void testEngineFuncsBaseCase()
{
  AAWorld engine;
  DISPLAY->setWindowTitle("AA Loop Test - Close Window To Continue");

  //1 on begin test
  auto myOnBeginFunc = []() {

    std::cout << "+++One Time Begin Run -> TESTING ALL ENGINE FUNCTION LOOPS\n";
  };
  engine.addToOnBegin(myOnBeginFunc);

  //2 on delta test1
  auto myChangeBackgroundBasedOnDeltaFunc = [](float step) {
    static float timePassed = 0;
    timePassed += step;
    float newcolor = abs(sin(timePassed));
    AADisplay::getInstance()->setWindowClearColor(
      glm::vec3(
        newcolor,
        newcolor,
        newcolor
      )
    );
  };
  engine.addToDeltaUpdate(myChangeBackgroundBasedOnDeltaFunc);
  auto myDeltaCoutFunc = [](float step) {
    std::cout << ".      process delta update: " << step << '\n'; };
  engine.addToDeltaUpdate(myDeltaCoutFunc);

  //3 on render test
  auto myRenderCoutFunc = []() {
    std::cout << "..     process render\n"; };
  engine.addToOnRender(myRenderCoutFunc);

  //4 on timeout input test
  auto timedKeys = [](AAKeyBoardInput& keys) {
    std::cout << "...    90ms timedout keyboard/mouse\n"; return false;  };
  engine.addToTimedOutKeyHandling(timedKeys);

  //4 on input test
  auto myButtonInputHandlerFunc = [](AAKeyBoardInput& keys) {
    std::cout << "....   process keyboard/mouse \n";  };
  engine.addToKeyHandling(myButtonInputHandlerFunc);

  //5 on mouse scroll test
  auto myMouseScrollWheelHandlerFunc = [](AAScrollInput& scroll) {
    std::cout << ".....  process mouse scroll: X:" << scroll.xOffset << " Y:" << scroll.yOffset << '\n'; };
  engine.addToScrollHandling(myMouseScrollWheelHandlerFunc);

  //6 mouse movement test
  auto myMouseMovementFunc = [](AAMouseInput& pointer) {
    std::cout << "...... process mouse pointer: X:" << pointer.xOffset << " Y:" << pointer.yOffset << '\n';  };
  engine.addToMouseHandling(myMouseMovementFunc);



  //8 update
  auto updatecout = []() {
    std::cout << ".......process final update -- END LOOP\n"; };
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