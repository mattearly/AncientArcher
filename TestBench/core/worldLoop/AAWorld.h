/***
 * AAWorld class - AKA the World Data Cruncher!
 * bringing it all together in loops to represent time passing and
 * processing time options during the loop.
**/

#pragma once
#include <glad\glad.h>
#include "../winSys/AADisplay.h"
#include "../winSys/AAInput.h"
#include "../winSys/AAControls.h"
#include "../objectSys/AASkybox.h"
#include "../objectSys/AAGameObject.h"
#include <vector>
#include <functional>
#include <memory>

#define DISPLAY AADisplay::getInstance()
#define CONTROLS AAControls::getInstance()

class AAWorld
{
public:
  AAWorld();
  AAWorld(const char* title);

  // getters
  const AACamera& getCamera(int camId) const;
  AACamera& getCamera(int camId);

  const AAOGLShader& getShader(int shadId) const;
  AAOGLShader& getShader(int shadId);

  const AAGameObject& getGameObject(int objId) const;
  AAGameObject& getGameObject(int objId);

  // add to the world to use while processing data, these return a unique id each add for the object type
  int addCamera();
  int addShader(const char* vert_path, const char* frag_path);
  int addObject(const char* path, int cam_id, int shad_id);

  // functions the engine loops through
  void addToOnBegin(void(*function)());
  void addToDeltaUpdate(void(*function)(float));
  void addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&));
  void addToKeyHandling(void(*function)(AAKeyBoardInput&));
  void addToScrollHandling(void(*function)(AAScrollInput&));
  void addToMouseHandling(void(*function)(AAMouseInput&));
  void addToUpdate(void(*function)());
  void addToSlowUpdate(void(*function)());

  // begin run through the functions
  int runMainLoop();
  void shutdown();

  MouseReporting getMouseHandlingType();

  // call during the world run
  void setSkybox(const std::shared_ptr<AASkybox>& skybox) noexcept;
  void setToPerspectiveMouseHandling();
  void setToStandardMouseHandling();
  void setKeyTimeoutLength(float time) noexcept;
  void setWindowTitle(const char* title);
  void setCursorToDisabled();
  void setCursorToEnabled();
  void setRenderDistance(int camId, float amt);
  void setProjectionMatrix(int camId, int shadId);

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

  std::vector<AACamera> mCameras;
  std::vector<AAOGLShader> mShaders;
  std::vector<AAGameObject> mGameObjects;

  std::vector<void (*)()> onBegin;
  std::vector<void (*)(float)> onDeltaUpdate;
  std::vector<void (*)(AAKeyBoardInput&)> onKeyHandling;
  std::vector<bool (*)(AAKeyBoardInput&)> onTimeoutKeyHandling;
  std::vector<void (*)(AAScrollInput&)> onScrollHandling;
  std::vector<void (*)(AAMouseInput&)> onMouseHandling;
  std::vector<void (*)()> onUpdate;
  std::vector<void (*)()> onSlowUpdate;

  void initEngine();
  void initDisplay();
  void resetEngine() noexcept;

};

