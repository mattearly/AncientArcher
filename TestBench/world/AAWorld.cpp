#include "AAWorld.h"
#include <vector>
#include <string>
#include <iostream>
#include "AAControls.h"
#include <GLFW/glfw3.h>

AAWorld::AAWorld()
{
  initEngine();
}

AAWorld::AAWorld(const char* title)
{
  initEngine();
  setWindowTitle(title);
}

AAWorld::~AAWorld()
{
  stop();
}

int AAWorld::run()
{
  begin();
  while (!glfwWindowShouldClose(DISPLAY->getWindow()))
  {
    static float currentFrame(0.f), deltaTime(0.f), lastFrame(0.f);
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    mEngineRunTimer += deltaTime;

    update(deltaTime);

    render();

    update();
  }
  return 0;
}

void AAWorld::shutdown()
{
  DISPLAY->closeWindow();
}

void AAWorld::addToOnBegin(void(*function)())
{
  onBegin.push_back(function);
}

void AAWorld::addToUpdate(void(*function)())
{
  onUpdate.push_back(function);
}

void AAWorld::addToSlowUpdate(void(*function)())
{
  onSlowUpdate.push_back(function);
}

void AAWorld::addToKeyHandling(void(*function)(AAKeyBoardInput&))
{
  onKeyHandling.push_back(function);
}

void AAWorld::addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&))
{
  onTimeoutKeyHandling.push_back(function);
}

void AAWorld::addToScrollHandling(void(*function)(AAScrollInput&))
{
  onScrollHandling.push_back(function);
}

void AAWorld::addToMouseHandling(void(*function)(AAMouseInput&))
{
  onMouseHandling.push_back(function);
}

void AAWorld::setSkybox(const std::shared_ptr<AASkybox>& skybox)
{
  mSkybox = skybox;
}

void AAWorld::setToPerspectiveMouseHandling()
{
  if (getMouseHandlingType() != MouseReporting::PERSPECTIVE)
  {
    DISPLAY->setCurorPosToPerspectiveCalc();
  }
  else
  {
    std::cout << "engine already in PERSPECTIVE mouse calc\n";
  }
}

void AAWorld::setToStandardMouseHandling()
{
  if (getMouseHandlingType() != MouseReporting::STANDARD)
  {
    DISPLAY->setCurorPosToStandardCalc();
  }
  else
  {
    std::cout << "engine already in STANDARD mouse calc\n";
  }
}

void AAWorld::setKeyTimeoutLength(float time) noexcept
{
  mKeyTimeOutLength = time < 0.f ? 0.0001f : time;
}

void AAWorld::setWindowTitle(const char* title)
{
  DISPLAY->setWindowTitle(title);
}

void AAWorld::setRenderDistance(const float& to)
{
  VIEWPORT->setRenderDistance(to);
}

MouseReporting AAWorld::getMouseHandlingType()
{
  return DISPLAY->mMouseReporting;
}

void AAWorld::addToOnRender(void(*function)())
{
  onRender.push_back(function);
}

void AAWorld::addToDeltaUpdate(void(*function)(float))
{
  onDeltaUpdate.push_back(function);
}

void AAWorld::render()
{
  DISPLAY->clearBackBuffer();

  for (auto onRen : onRender) { onRen(); }
  if (mSkybox) { mSkybox->render(); }

  DISPLAY->swapWindowBuffers();
}

void AAWorld::begin()
{
  DISPLAY->keepWindowOpen();

  for (auto oB : onBegin)
  {
    oB();
  }
}

void AAWorld::update(float dt)
{
  for (auto oDU : onDeltaUpdate)
  {
    oDU(dt);
  }
}

void AAWorld::update()
{
  processSystemKeys();
  float passedTime = mEngineRunTimer - mTimeOutCheckStamp;

  mButtonTimeOutSoFar += passedTime;
  if (mButtonTimeOutSoFar > mKeyTimeOutLength)   //todo: move to delta update
  {
    int buttonUsed = false;
    if ((AAControls::getInstance()->mButtonState.leftAlt || AAControls::getInstance()->mButtonState.rightAlt) && AAControls::getInstance()->mButtonState.enter)
    {
      DISPLAY->toggleFullScreen();
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
      mButtonTimeOutSoFar = 0.f;
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

  if (getMouseHandlingType() == MouseReporting::PERSPECTIVE)
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
    for (auto oSU : onSlowUpdate)// todo: move to delta update
    {
      oSU();
    }
    worldUpdateTimeOutSoFar = 0.f;
  }

  mTimeOutCheckStamp = mEngineRunTimer;

}

void AAWorld::processSystemKeys()
{
  AAControls::getInstance()->pullButtonStateEvents();
}

void AAWorld::initEngine()
{
  initDisplay();
  mEngineRunTimer = 0.f;
  mSlowUpdateDelay = 1.f;
  mKeyTimeOutLength = 0.56667f;
  mTimeOutCheckStamp = 0.f;
  mButtonTimeOutSoFar = 0.f;
}

void AAWorld::initDisplay()
{
  DISPLAY->initFromEngine();
}

void AAWorld::stop()
{
  onBegin.clear();
  onDeltaUpdate.clear();
  onRender.clear();
  onUpdate.clear();
  onKeyHandling.clear();
  onScrollHandling.clear();
  onMouseHandling.clear();
  DISPLAY->resetStateDataToDefault();
}
