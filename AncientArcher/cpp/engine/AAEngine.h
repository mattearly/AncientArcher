#pragma once
#include "AADisplay.h"
#include "AAInput.h"
#include "AAViewport.h"
#include <memory>
#include <vector>
#include <functional>
class AAEngine
{
public:

  ~AAEngine();

  int run();
  void shutdown();

  void setKeyStruct(std::shared_ptr<AAKeyInput>& keys);
  void setMouseStruct(std::shared_ptr<AAMouseInput>& mouse);
  void setScrollStruct(std::shared_ptr<AAScrollInput>& scroll);

  void addToOnBegin(void(*function)());
  void addToDeltaUpdate(void(*function)(float));
  void addToOnRender(void(*function)());
  void addToUpdate(void(*function)());
  void addToKeyHandling(void(*function)(std::shared_ptr<AAKeyInput>&));
  void addToScrollHandling(void(*function)(std::shared_ptr<AAScrollInput>&));
  void addToMouseHandling(void(*function)(std::shared_ptr<AAMouseInput>&));

private:

  void begin();
  void update(float dt);
  void render();
  void update();

  std::shared_ptr<AAKeyInput> mKeys;
  std::shared_ptr<AAMouseInput> mMouse;
  std::shared_ptr<AAScrollInput> mScroll;

  std::vector<void (*)()> onBegin;
  std::vector<void (*)(float)> onDeltaUpdate;
  std::vector<void (*)()> onRender;
  std::vector<void (*)(std::shared_ptr<AAKeyInput>&)> onKeyHandling;
  std::vector<void (*)(std::shared_ptr<AAScrollInput>&)> onScrollHandling;
  std::vector<void (*)(std::shared_ptr<AAMouseInput>&)> onMouseHandling;
  std::vector<void (*)()> onUpdate;

  bool checkIfKeysSet();
  bool checkIfMouseSet();
  bool checkIfScrollSet();

};
