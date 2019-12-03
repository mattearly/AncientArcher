#include "AAEngine.h"

AAEngine::~AAEngine()
{
  onBegin.clear();
  onDeltaUpdate.clear();
  onRender.clear();
  onUpdate.clear();
  onKeyHandling.clear();
  onScrollHandling.clear();
  onMouseHandling.clear();
}

int AAEngine::run()
{
  if (checkIfKeysSet() == false)
  {
    return -1;
  }
  if (checkIfMouseSet() == false)
  {
    return -2;
  }
  if (checkIfScrollSet() == false)
  {
    return -3;
  }

  begin();

  AADisplay::getInstance()->keepWindowOpen();
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

void AAEngine::shutdown()
{
  AADisplay::getInstance()->closeWindow();
}

void AAEngine::setKeyStruct(std::shared_ptr<AAKeyInput>& keys)
{
  mKeys = keys;
}

void AAEngine::setMouseStruct(std::shared_ptr<AAMouseInput>& mouse)
{
  mMouse = mouse;
  AAControls::getInstance()->setMouse(mouse);
}

void AAEngine::setScrollStruct(std::shared_ptr<AAScrollInput>& scroll)
{
  mScroll = scroll;
  AAControls::getInstance()->setScroll(scroll);
}

void AAEngine::addToOnBegin(void(*function)())
{
  onBegin.push_back(function);
}

void AAEngine::addToUpdate(void(*function)())
{
  onUpdate.push_back(function);
}

void AAEngine::addToKeyHandling(void(*function)(std::shared_ptr<AAKeyInput>&))
{
  onKeyHandling.push_back(function);
}

void AAEngine::addToScrollHandling(void(*function)(std::shared_ptr<AAScrollInput>&))
{
  onScrollHandling.push_back(function);
}

void AAEngine::addToMouseHandling(void(*function)(std::shared_ptr<AAMouseInput>&))
{
  onMouseHandling.push_back(function);
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

  for (auto ren : onRender)
  {
    ren();
  }

  AADisplay::getInstance()->swapWindowBuffers();
}

void AAEngine::begin()
{
  for (auto ob : onBegin)
  {
    ob();
  }
}

void AAEngine::update(float dt)
{
  for (auto ud : onDeltaUpdate)
  {
    ud(dt);
  }
}

void AAEngine::update()
{
  glfwPollEvents();
  AAControls::getInstance()->keyInput(mKeys);
  for (auto okh : onKeyHandling)
  {
    okh(mKeys);
  }

  for (auto osh : onScrollHandling)
  {
    osh(mScroll);
  }

  for (auto omh : onMouseHandling)
  {
    omh(mMouse);
  }

  mMouse->xOffset = 0;
  mMouse->yOffset = 0;

  for (auto u : onUpdate)
  {
    u();
  }

  static float checkStamp = 0.f;
  float passedTime = mEngineRunTimer - checkStamp;
  static float accumulatedTime = 0.f;
  accumulatedTime += passedTime;

  if (accumulatedTime > 1.f && (mKeys->leftAlt || mKeys->rightAlt) && mKeys->enter)
  {
    AADisplay::getInstance()->toggleFullScreen();
    checkStamp = mEngineRunTimer;
    accumulatedTime = 0.f;
  }
}

bool AAEngine::checkIfKeysSet()
{
  return mKeys ? true : false;
}

bool AAEngine::checkIfMouseSet()
{
  return mMouse ? true : false;
}

bool AAEngine::checkIfScrollSet()
{
  return mScroll ? true : false;
}
