#pragma once
#include "AADisplay.h"
#include "AAKeyInput.h"
#include "AAViewport.h"
#include <memory>
class AAEngine
{
public:
  int run();

  void setKeyStruct(std::shared_ptr<AAKeyInput>& keys);

private:
  void render();

  void update(float dt);

  void update();

  std::shared_ptr<AAKeyInput> mKeys;

  int checkIfKeysSet();

};
