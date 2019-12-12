#pragma once
#include "AADisplay.h"
#include "AAInput.h"
#include "AAViewport.h"
#include "AASkybox.h"
#include <vector>
#include <functional>
#include <memory>
class AAEngine
{
public:

  AAEngine();
  ~AAEngine();

  int run();
  void shutdownEngine();

  void addToOnBegin(void(*function)());
  void addToDeltaUpdate(void(*function)(float));
  void addToOnRender(void(*function)());
  void addToUpdate(void(*function)());
  void addToKeyHandling(void(*function)(AAKeyBoardInput&));
  void addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&));
  void addToScrollHandling(void(*function)(AAScrollInput&));
  void addToMouseHandling(void(*function)(AAMouseInput&));

  friend class AAControls;

private:

  void begin();
  void update(float dt);
  void render();
  void update();

  void processSystemKeys();

  std::vector<void (*)()> onBegin;
  std::vector<void (*)(float)> onDeltaUpdate;
  std::vector<void (*)()> onRender;
  std::vector<void (*)(AAKeyBoardInput&)> onKeyHandling;
  std::vector<bool (*)(AAKeyBoardInput&)> onTimeoutKeyHandling;
  std::vector<void (*)(AAScrollInput&)> onScrollHandling;
  std::vector<void (*)(AAMouseInput&)> onMouseHandling;
  std::vector<void (*)()> onUpdate;

  float mEngineRunTimer = 0.f;

  const float mKeyTimeOutLength = 1.021f;

  bool mDisplayInitiated = false;
  void initDisplay();

  std::unique_ptr<AASkybox> mSkybox;
};
