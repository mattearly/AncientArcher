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

  const MouseReporting getMouseReportingMode() const;
  const int getCursorMode() const;

  // add to the world to use while processing data, these return a unique id each add for the object type
  int addCamera();
  int addShader(const char* vert_path, const char* frag_path);
  int addObject(const char* path, int cam_id, int shad_id);
  int addObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details);

  void setSkybox(const std::shared_ptr<AASkybox>& skybox) noexcept;
  void setRenderDistance(const float& amt, int camId);

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


  // call as needed during the world run
  void setCursorToDisabled();
  void setToPerspectiveMouseHandling();

  void setCursorToEnabled(bool isHardwareRendered = false);
  void setToStandardMouseHandling();

  void setWindowTitle(const char* title);

  void setRenderDistance(int camId, float amt);
  void setProjectionMatrix(int shadId, int camId);

  friend class AAControls;

private:

  void begin(); // ran once on run

  void setProjectionMatToAllShadersFromFirstCam_hack();  //used for testing purposes until more elegant solution appears

  // used in main loop
  void update(float dt);
  void render();
  void update();
  void processSystemKeys();

  // mNonSpammableKeysTimeout keeps track of how long the keys have timed out
  float mNonSpammableKeysTimeout;
  // how long the non-spammable keys are to time out for at least
  float mNoSpamWaitLength;

  // mSlowUpdateTimeout keeps track of how how long the slow update has been timed out
  float mSlowUpdateTimeout;
  // how long the slow update times out for at least
  float mSlowUpdateWaitLength;

  std::vector<AACamera>     mCameras;
  std::vector<AAOGLShader>  mShaders;
  std::vector<AAGameObject> mGameObjects;

  std::shared_ptr<AASkybox> mSkybox;

  std::vector<void (*)()>                 onBegin;
  std::vector<void (*)(float)>            onDeltaUpdate;
  std::vector<void (*)()>                 onSlowDeltaUpdate;
  std::vector<void (*)()>                 onUpdate;
  std::vector<void (*)(AAKeyBoardInput&)> onKeyHandling;
  std::vector<void (*)(AAScrollInput&)>   onScrollHandling;
  std::vector<void (*)(AAMouseInput&)>    onMouseHandling;
  std::vector<bool (*)(AAKeyBoardInput&)> onTimeoutKeyHandling;

  // helpers
  void initEngine();
  void initDisplay();
  void resetEngine() noexcept;

};

