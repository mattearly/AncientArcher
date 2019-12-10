#pragma once
#include "AADisplay.h"
#include "AAInput.h"
#include "AAViewport.h"
#include <vector>
#include <functional>
class AAEngine
{
public:

  ~AAEngine();

  int run();
  void shutdownEngine();

  void addToOnBegin(void(*function)());
  void addToDeltaUpdate(void(*function)(float));
  void addToOnRender(void(*function)());
  void addToUpdate(void(*function)());
  void addToKeyHandling(void(*function)(AAKeyBoardInput&));
  void addToScrollHandling(void(*function)(AAScrollInput&));
  void addToMouseHandling(void(*function)(AAMouseInput&));

  friend class AAControls;

private:

  void begin();
  void update(float dt);
  void render();
  void update();

  void processSystemHotKeys();

  std::vector<void (*)()> onBegin;
  std::vector<void (*)(float)> onDeltaUpdate;
  std::vector<void (*)()> onRender;
  std::vector<void (*)(AAKeyBoardInput&)> onKeyHandling;
  std::vector<void (*)(AAScrollInput&)> onScrollHandling;
  std::vector<void (*)(AAMouseInput&)> onMouseHandling;
  std::vector<void (*)()> onUpdate;

  float mEngineRunTimer = 0.f;

};
