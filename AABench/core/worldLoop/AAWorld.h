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

  float mLastFrame;               ///< keeps track of the previous frame's time
  float mCurrentFrame;            ///< keeps track of the current frame's time
  float mDeltaTime;               ///< the delta difference between previous and current frame
  float mNonSpammableKeysTimeout; ///< keeps track of how long the keys have timed out
  float mNoSpamWaitLength;        ///< how long the non-spammable keys are to time out for at least
  float mSlowUpdateTimeout;       ///< keeps track of how how long the slow update has been timed out
  float mSlowUpdateWaitLength;    ///< ms length the slow update times out for at least

  std::vector<AACamera>     mCameras;     ///< list of available cameras
  std::vector<AAOGLShader>  mShaders;     ///< list of available shaders
  std::vector<AAGameObject> mGameObjects; ///< list of available objects
  std::shared_ptr<AASkybox> mSkybox;      ///< the main skybox

  std::vector<std::function<void()> >                 onBegin;              ///< list of functions to run once when runMainLoop is called
  std::vector<std::function<void(float)> >            onDeltaUpdate;        ///< list of functions that rely on deltatime in the main loop
  std::vector<std::function<void()> >                 onUpdate;             ///< list of functions that run every frame in the main loop
  std::vector<std::function<void()> >                 onSlowDeltaUpdate;    ///< list of functions to run every only every mSlowUpdateWaitLength in the main loop
  std::vector<std::function<void(AAKeyBoardInput&)> > onKeyHandling;        ///< list of functions to handle keypresses every frame in the main loop
  std::vector<std::function<void(AAScrollInput&)> >   onScrollHandling;     ///< list of functions to handle mouse wheel scroll every frame in the main loop
  std::vector<std::function<void(AAMouseInput&)> >    onMouseHandling;      ///< list of functions to handle mouse movement every frame in the main loop
  std::vector<std::function<bool(AAKeyBoardInput&)> > onTimeoutKeyHandling; ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press

  void begin();
  void deltaUpdate();
  void render();
  void afterRenderUpdate();

  // helpers
  void initEngine();
  void resetEngine() noexcept;
  void setProjectionMatToAllShadersFromFirstCam_hack();  //used for testing purposes until more elegant solution appears

};

