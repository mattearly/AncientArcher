#include "AAEngine.h"
#include <vector>
#include <string>

AAEngine::AAEngine()
{
  initDisplay();
  int skybox_choice = 1;

  const int numOptions = 2;
  std::string name[numOptions] = { "nordic","drakeq" };
  assert(skybox_choice < numOptions);
  {
    // this is all based on where I am storing the data for cubemaps for testing
    std::string folderpath = "C:/Users/matt/Dropbox_me298414/Dropbox/SkyboxCubemaps/";
    std::string order[6] = { "/right", "/left", "/up", "/down", "/front", "/back" };
    std::string extension = ".png";

    std::vector<std::string> cubemapfiles;
    for (int j = 0; j < 6; ++j)
    {
      cubemapfiles.emplace_back(folderpath + name[skybox_choice] + order[j] + extension);
    }
    mSkybox = std::make_unique<AASkybox>(cubemapfiles);
  }
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
}

int AAEngine::run()
{
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

  static float checkStamp = 0.f;
  float passedTime = mEngineRunTimer - checkStamp;
  static float systemKeyTimeOut = 0.f;
  systemKeyTimeOut += passedTime;
  if (systemKeyTimeOut > mKeyTimeOutLength)
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
      systemKeyTimeOut = 0.f;
    }

  }

  checkStamp = mEngineRunTimer;

  for (auto oKH : onKeyHandling)
  {
    oKH(AAControls::getInstance()->mButtonState);
  }

  for (auto oSH : onScrollHandling)
  {
    oSH(AAControls::getInstance()->mMouseWheelScroll);
  }

  for (auto oMH : onMouseHandling)
  {
    oMH(AAControls::getInstance()->mMousePosition);
  }

  AAControls::getInstance()->mMousePosition.xOffset = 0;
  AAControls::getInstance()->mMousePosition.yOffset = 0;

  for (auto oU : onUpdate)
  {
    oU();
  }

}

void AAEngine::processSystemKeys()
{
  AAControls::getInstance()->pullButtonStateEvents();
}

void AAEngine::initDisplay()
{
  AADisplay::getInstance()->initFromEngine();
}
