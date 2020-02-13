/***
 * AAWorld class - AKA the World Data Cruncher!
 * bringing it all together in loops to represent time passing and 
 * processing time options during the loop.
**/

#pragma once
#include "AADisplay.h"
#include "AAInput.h"
#include "AAViewport.h"
#include "AASkybox.h"
#include <vector>
#include <functional>
#include <memory>

#define DISPLAY AADisplay::getInstance()
#define CONTROLS AAControls::getInstance()
#define VIEWPORT AAViewport::getInstance()

class AAWorld
{
public:

  AAWorld();
  ~AAWorld();

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
  void setKeyTimeoutLength(float time);

  MouseReporting getMouseHandlingType();

  friend class AAControls;

private:

  void begin();
  void update(float dt);
  void render();
  void update();

  void processSystemKeys();

  std::shared_ptr<AASkybox> mSkybox;

  float mEngineRunTimer;
  float mSlowUpdateDelay;
  float mKeyTimeOutLength;
  float mTimeOutCheckStamp;
  float mButtonTimeOutSoFar;

  glm::mat4 m_WorldTransform;

  std::vector<void (*)()> onBegin;
  std::vector<void (*)(float)> onDeltaUpdate;
  std::vector<void (*)()> onRender;
  std::vector<void (*)(AAKeyBoardInput&)> onKeyHandling;
  std::vector<bool (*)(AAKeyBoardInput&)> onTimeoutKeyHandling;
  std::vector<void (*)(AAScrollInput&)> onScrollHandling;
  std::vector<void (*)(AAMouseInput&)> onMouseHandling;
  std::vector<void (*)()> onUpdate;
  std::vector<void (*)()> onSlowUpdate;

  void initDisplay();

};
