#include "AAEngine.h"
#include <iostream>

void AAEngine::run()
{
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



  AADisplay::getInstance()->swapWindowBuffers();
}

void AAEngine::update(float dt)
{
}

void AAEngine::update()
{
  glfwPollEvents();
  AAControls::getInstance()->keyInput(mKeys.get());


  if (mKeys->a)
  {
    std::cout << "a pressed\n";
  }

  static bool isFullscreen = false;
  if (mKeys->leftAlt && mKeys->enter)
  {
    if (isFullscreen)
    {
      AADisplay::getInstance()->FullscreenOff();
      isFullscreen = !isFullscreen;
    }
    else
    {
      AADisplay::getInstance()->FullscreenOn();
      isFullscreen = !isFullscreen;
    }
  }
}
