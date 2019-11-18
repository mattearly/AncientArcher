#include "AAEngine.h"

int AAEngine::run()
{
  if (checkIfKeysSet() == -1)
  {
    return -1;
  }
  while (!glfwWindowShouldClose(AADisplay::getInstance()->getWindow()))
  {
    static float currentFrame(0.f), deltaTime(0.f), lastFrame(0.f);
    currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    update(deltaTime);

    render();

    update();
  }
}

void AAEngine::setKeyStruct(std::shared_ptr<AAKeyInput>& keys)
{
  mKeys = keys;
}

void AAEngine::render()
{
  AADisplay::getInstance()->clearBackBuffer();

  // display new draws
  

  AADisplay::getInstance()->swapWindowBuffers();
}

void AAEngine::update(float dt)
{
}

void AAEngine::update()
{
  glfwPollEvents();
  AAControls::getInstance()->keyInput(mKeys.get());
  if ((mKeys->leftAlt || mKeys->rightAlt) && mKeys->enter)
  {
    if (AADisplay::getInstance()->getIsWindowFullScreen())
    {
      AADisplay::getInstance()->setFullscreenToOff();
    }
    else
    {
      AADisplay::getInstance()->setFullscreenToOn();
    }
  }
}

int AAEngine::checkIfKeysSet()
{
  if (!mKeys)
  {
    return -1;
  }
}
