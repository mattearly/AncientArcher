#include "AAEngine.h"
#include <vector>
#include <string>
#include <iostream>

AAEngine::AAEngine()
{
  initDisplay();
}

AAEngine::~AAEngine()
{
  onBegin.clear();
  onDeltaUpdate.clear();
  onRender.clear();
  onUpdate.clear();
  onKeyHandling.clear();
  onScrollHandling.clear();
  onMouseHandling.clear();
  glfwDestroyWindow(AADisplay::getInstance()->getWindow());
}

int AAEngine::run()
{
  begin();
  while (!glfwWindowShouldClose(AADisplay::getInstance()->getWindow()))
  {
    static float currentFrame(0.f), deltaTime(0.f), lastFrame(0.f);
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    mEngineRunTimer += deltaTime;

    update(deltaTime);

    render();

    update();
  }
  return 0;
}

void AAEngine::shutdownEngine()
{
  AADisplay::getInstance()->closeWindow();
}

void AAEngine::addToOnBegin(void(*function)())
{
  onBegin.push_back(function);
}

void AAEngine::addToUpdate(void(*function)())
{
  onUpdate.push_back(function);
}

void AAEngine::addToSlowUpdate(void(*function)())
{
  onSlowUpdate.push_back(function);
}

void AAEngine::addToKeyHandling(void(*function)(AAKeyBoardInput&))
{
  onKeyHandling.push_back(function);
}

void AAEngine::addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&))
{
  onTimeoutKeyHandling.push_back(function);
}

void AAEngine::addToScrollHandling(void(*function)(AAScrollInput&))
{
  onScrollHandling.push_back(function);
}

void AAEngine::addToMouseHandling(void(*function)(AAMouseInput&))
{
  onMouseHandling.push_back(function);
}

void AAEngine::setSkybox(const std::shared_ptr<AASkybox>& skybox)
{
  mSkybox = skybox;
}

void AAEngine::setToPerspectiveMouseHandling()
{
  if (mMouseReporting != MouseReporting::PERSPECTIVE)
  {
    mMouseReporting = MouseReporting::PERSPECTIVE;
    AADisplay::getInstance()->setCurorPosToPerspectiveCalc();
  }
  else
  {
    std::cout << "engine already in PERSPECTIVE mouse calc\n";
  }
}

void AAEngine::setToStandardMouseHandling()
{
  if (mMouseReporting != MouseReporting::STANDARD)
  {
    mMouseReporting = MouseReporting::STANDARD;
    AADisplay::getInstance()->setCurorPosToStandardCalc();
  }
  else
  {
    std::cout << "engine already in STANDARD mouse calc\n";
  }
}

void AAEngine::addToOnRender(void(*function)())
{
  onRender.push_back(function);
}

void AAEngine::addToDeltaUpdate(void(*function)(float))
{
  onDeltaUpdate.push_back(function);
}

void AAEngine::render()
{
  AADisplay::getInstance()->clearBackBuffer();

  for (auto onRen : onRender) { onRen(); }
  if (mSkybox) { mSkybox->render(); }

  AADisplay::getInstance()->swapWindowBuffers();
}

void AAEngine::begin()
{
  AADisplay::getInstance()->keepWindowOpen();

  for (auto oB : onBegin)
  {
    oB();
  }
}

void AAEngine::update(float dt)
{
  for (auto oDU : onDeltaUpdate)
  {
    oDU(dt);
  }
}

void AAEngine::update()
{
  processSystemKeys();
  static float timeOutCheckStamp = 0.f;
  float passedTime = mEngineRunTimer - timeOutCheckStamp;

  static float buttonTimeOutSoFar = 0;
  buttonTimeOutSoFar += passedTime;
  if (buttonTimeOutSoFar > mKeyTimeOutLength)
  {
    int buttonUsed = false;
    if ((AAControls::getInstance()->mButtonState.leftAlt || AAControls::getInstance()->mButtonState.rightAlt) && AAControls::getInstance()->mButtonState.enter)
    {
      AADisplay::getInstance()->toggleFullScreen();
      buttonUsed = true;
    }

    for (auto oTKH : onTimeoutKeyHandling)
    {
      if (oTKH(AAControls::getInstance()->mButtonState))
      {
        buttonUsed = true;
      }
    }

    if (buttonUsed)
    {
      buttonTimeOutSoFar = 0.f;
    }
  }

  for (auto oKH : onKeyHandling)
  {
    oKH(AAControls::getInstance()->mButtonState);
  }

  for (auto oSH : onScrollHandling)
  {
    oSH(AAControls::getInstance()->mMouseWheelScroll);
  }

  AAControls::getInstance()->mMouseWheelScroll.xOffset = 0;
  AAControls::getInstance()->mMouseWheelScroll.yOffset = 0;

  for (auto oMH : onMouseHandling)
  {
    oMH(AAControls::getInstance()->mMousePosition);
  }

  if (mMouseReporting == MouseReporting::PERSPECTIVE)
  {
    AAControls::getInstance()->mMousePosition.xOffset = 0;
    AAControls::getInstance()->mMousePosition.yOffset = 0;
  }

  for (auto oU : onUpdate)
  {
    oU();
  }

  static float worldUpdateTimeOutSoFar = 0;
  worldUpdateTimeOutSoFar += passedTime;
  if (worldUpdateTimeOutSoFar > mSlowUpdateDelay)
  {
    for (auto oSU : onSlowUpdate)
    {
      oSU();
    }
    worldUpdateTimeOutSoFar = 0.f;
  }

  timeOutCheckStamp = mEngineRunTimer;

}

void AAEngine::processSystemKeys()
{
  AAControls::getInstance()->pullButtonStateEvents();
}

void AAEngine::initDisplay()
{
  AADisplay::getInstance()->initFromEngine();
}
