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
  void addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&));
  void addToKeyHandling(void(*function)(AAKeyBoardInput&));
  void addToScrollHandling(void(*function)(AAScrollInput&));
  void addToMouseHandling(void(*function)(AAMouseInput&));
  void addToUpdate(void(*function)());
  void addToSlowUpdate(void(*function)());

  void setSkybox(const std::shared_ptr<AASkybox>& skybox);
  void setToPerspectiveMouseHandling();
  void setToStandardMouseHandling();

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
  std::vector<void (*)()> onSlowUpdate;

  float mEngineRunTimer = 0.f;

  void initDisplay();

  std::shared_ptr<AASkybox> mSkybox;

  float mSlowUpdateDelay = 1.f;
  const float mKeyTimeOutLength = 0.56667f;

  enum class MouseReporting { STANDARD, PERSPECTIVE } mMouseReporting = MouseReporting::STANDARD;

};
