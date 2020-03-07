#include "AAWorld.h"
#include "../winSys/AAControls.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
//#include <iostream>

int AAWorld::runMainLoop()
{
  begin();
  while (!glfwWindowShouldClose(DISPLAY->getWindow()))
  {
    static float lastFrame(0.f);
    const float currentFrame = static_cast<float>(glfwGetTime());
    const float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    update(deltaTime);
    render();
    update();
  }
  resetEngine();
  return 0;
}

void AAWorld::shutdown()
{
  //DISPLAY->resetStateDataToDefault();
  DISPLAY->closeWindow();
}

void AAWorld::addToDeltaUpdate(void(*function)(float))
{
  onDeltaUpdate.push_back(function);
}

void AAWorld::render()
{
  DISPLAY->clearBackBuffer();

  for (auto& obj : mGameObjects)
  {
    // get the id of the shader for this object
    const int shaderID = obj.getShaderId();

    // switch to the shader
    getShader(shaderID).use();

    // get the camera id for this object
    const int cameraID = obj.getCameraId();

    // set the view matrix from the cam for this object
    getShader(shaderID).setMat4("view", getCamera(cameraID).getViewMatrix());

    obj.draw(getShader(shaderID));
  }

  if (mSkybox) { mSkybox->render(mCameras.front()); }

  DISPLAY->swapWindowBuffers();
}

void AAWorld::begin()
{
  DISPLAY->keepWindowOpen();

  for (auto& oB : onBegin)
  {
    oB();
  }

  setProjectionMatToAllShadersFromFirstCam_hack();

}

void AAWorld::setProjectionMatToAllShadersFromFirstCam_hack()
{
  // set proj matries hack from first cam
  for (auto& shad : mShaders)
  {
    //shad.setMat4("projection", mCameras.front().getProjectionMatrix());
    const int sID = shad.getID();
    //std::cout << "shad id: " << sID << '\n';

    const int cID = mCameras.front().getID();
    //std::cout << "shad id: " << sID << '\n';

    setProjectionMatrix(sID, cID);  //set shader (sID) from cam (cID)
  }

  // if there is a skybox
  if (mSkybox)
  {
    // if there is a camera
    if (mCameras.size() > 0) {
      // set the projection matrix on the skybox from the first cam proj matrix
      mSkybox->setProjectionMatrix(mCameras.front());
    }
  }

}

void AAWorld::update(float dt)
{
  for (auto& oDU : onDeltaUpdate)
  {
    oDU(dt);
  }

  // delayed updates for things you don't want spammed.
  // update accum time for delayed updates
  mSlowUpdateTimeout += dt;
  // check to see if its time to process delayed updates
  if (mSlowUpdateTimeout > mSlowUpdateWaitLength)
  {
    // process all delayed updates
    for (auto& oSU : onSlowDeltaUpdate)
    {
      oSU();
    }
    // we should also process whether the window size changed here
    if (DISPLAY->mWindowSizeChanged)
    {
      // notify shader/cams
      setProjectionMatToAllShadersFromFirstCam_hack();

      // don't process again
      DISPLAY->mWindowSizeChanged = false;
    }
    // reset timeout length to 0
    mSlowUpdateTimeout = 0.f;
  }

  // needs updated, we'll use it in update with keyboard functions before the loop is done. 
  // note that the keyboard processing cant be here because we have yet to run processSystemKeys()
  mNonSpammableKeysTimeout += dt;

}

void AAWorld::update()
{
  processSystemKeys();

  // absorb keyboard buttons
  for (auto& oKH : onKeyHandling) {oKH(CONTROLS->mButtonState);}

  // absorb scroll wheel
  for (auto& oSH : onScrollHandling) {oSH(CONTROLS->mMouseWheelScroll);}
  // reset scroll wheel to 0's after processing
  CONTROLS->mMouseWheelScroll.xOffset = 0;
  CONTROLS->mMouseWheelScroll.yOffset = 0;

  // handle mouse position
  for (auto& oMH : onMouseHandling) {oMH(CONTROLS->mMousePosition);}
  // Snap cursor to the middle of the screen if it is in perspective and cursor is disabled (FPP mode)
  if (getMouseReportingMode() == MouseReporting::PERSPECTIVE && getCursorMode() == GLFW_CURSOR_DISABLED)
  {
    //std::cout << " in perspective calc \n";
    //deb(CONTROLS->mMousePosition.xOffset);
    //deb(CONTROLS->mMousePosition.yOffset);

    CONTROLS->mMousePosition.xOffset = 0; 
    CONTROLS->mMousePosition.yOffset = 0;
  }
  else if (getMouseReportingMode() == MouseReporting::STANDARD && getCursorMode() == GLFW_CURSOR_NORMAL) // standard and normal
  {
    // dont snap the position
    //std::cout << " in standard mouse reporting calc\n";
    //deb(CONTROLS->mMousePosition.xOffset);
    //deb(CONTROLS->mMousePosition.yOffset);
  }

  // run through user prefered updates
  for (auto& oU : onUpdate) {oU();}

  // only be executable after a timeout has been met, sort of like a cooldown
  if (mNonSpammableKeysTimeout > mNoSpamWaitLength)
  {
    // process unspammable keys
    for (auto& oTOK : onTimeoutKeyHandling)
    {
      // if we get a true we stop processing
      if (oTOK(CONTROLS->mButtonState)) {
        //std::cout << "timeout key press detected. reseting timeoutkeytimer\n";
        mNonSpammableKeysTimeout = 0.f;
        break;
      }
    }
  }
}

void AAWorld::processSystemKeys()
{
  CONTROLS->pullButtonStateEvents();
}

void AAWorld::initEngine()
{
  resetEngine();

  initDisplay();

  // Init OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context) -- must be after initDisplay
  {
    //std::cout << "failed to init glad @ file " __FILE__ << " line " << __LINE__ << '\n';
    exit(-1);
  }
}

void AAWorld::initDisplay()
{
  DISPLAY->initFromEngine();
}

void AAWorld::resetEngine() noexcept
{
  //clear all vectors
  mCameras.clear();
  mShaders.clear();
  mGameObjects.clear();
  onBegin.clear();
  onDeltaUpdate.clear();
  onKeyHandling.clear();
  onTimeoutKeyHandling.clear();
  onScrollHandling.clear();
  onMouseHandling.clear();
  onUpdate.clear();
  onSlowDeltaUpdate.clear();

  // reset all state data
  mNonSpammableKeysTimeout = 0.f;
  mSlowUpdateTimeout = 0.f;
  mNoSpamWaitLength = .5667f;
  mSlowUpdateWaitLength = .5667f;
}

AAWorld::AAWorld()
{
  initEngine();
}

AAWorld::AAWorld(const char* title)
{
  initEngine();
  setWindowTitle(title);
}

int AAWorld::addObject(const char* path, int camId, int shadId)
{
  AAGameObject tmpObject(path, camId, shadId);
  const int return_id = tmpObject.getObjectId();

  mGameObjects.push_back(tmpObject);

  return return_id;
}

int AAWorld::addObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details)
{
  // todo: optimize. check if it is an object we already have loaded and use it again if so

  AAGameObject tmpObject(path, cam_id, shad_id, details);

  const int return_id = tmpObject.getObjectId();

  mGameObjects.push_back(tmpObject);

  return return_id;
}

int AAWorld::addCamera()
{
  AACamera tmpCam;
  const int return_id = tmpCam.getID();

  mCameras.push_back(tmpCam);

  return return_id;
}

int AAWorld::addShader(const char* vert_path, const char* frag_path)
{
  const AAOGLShader tmpShader(vert_path, frag_path);
  const int return_id = tmpShader.getID();
  mShaders.push_back(tmpShader);
  return return_id;
}

const AACamera& AAWorld::getCamera(int camId) const
{
  for (const auto& cam : mCameras)
  {
    if (cam.getID() == camId)
    {
      return cam;
    }
  }

  // if it didn't find it and return above ^^^^^^^^  show error message in console
  //std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get const AACamera\n";
  exit(-1);
}

AACamera& AAWorld::getCamera(int camId)
{
  for (auto& cam : mCameras)
  {
    if (cam.getID() == camId)
    {
      return cam;
    }
  }

  // if it didn't find it and return above ^^^^^^^^  show error message in console
  //std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get AACamera\n";
  exit(-1);
}

const AAOGLShader& AAWorld::getShader(int shadId) const
{
  for (const auto& shad : mShaders)
  {
    if (shad.getID() == shadId)
    {
      return shad;
    }
  }

  // if it didn't find it and return above ^^^^^^^^  show error message in console
  //std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
  exit(-1);
}

AAOGLShader& AAWorld::getShader(int shadId)
{
  for (auto& shad : mShaders)
  {
    if (shad.getID() == shadId)
    {
      return shad;
    }
  }

  // if it didn't find it and return above ^^^^^^^^  show error message in console
  //std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
  exit(-1);
}

const AAGameObject& AAWorld::getGameObject(int objId) const
{
  for (const auto& obj : mGameObjects)
  {
    if (obj.getObjectId() == objId)
    {
      return obj;
    }
  }

  // if it didn't find it and return above ^^^^^^^^  show error message in console
  //std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
  exit(-1);
}

AAGameObject& AAWorld::getGameObject(int objId)
{
  for (auto& obj : mGameObjects)
  {
    if (obj.getObjectId() == objId)
    {
      return obj;
    }
  }

  // if it didn't find it and return above ^^^^^^^^  show error message in console
  //std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
  exit(-1);
}

const MouseReporting AAWorld::getMouseReportingMode() const
{
  return DISPLAY->mMouseReporting;
}

const int AAWorld::getCursorMode() const
{
  return DISPLAY->getCursorMode();
}

void AAWorld::addToOnBegin(void(*function)())
{
  onBegin.push_back(function);
}

void AAWorld::addToUpdate(void(*function)())
{
  onUpdate.push_back(function);
}

void AAWorld::addToSlowUpdate(void(*function)())
{
  onSlowDeltaUpdate.push_back(function);
}

void AAWorld::addToKeyHandling(void(*function)(AAKeyBoardInput&))
{
  onKeyHandling.push_back(function);
}

void AAWorld::addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&))
{
  onTimeoutKeyHandling.push_back(function);
}

void AAWorld::addToScrollHandling(void(*function)(AAScrollInput&))
{
  onScrollHandling.push_back(function);
}

void AAWorld::addToMouseHandling(void(*function)(AAMouseInput&))
{
  onMouseHandling.push_back(function);
}

void AAWorld::setSkybox(const std::shared_ptr<AASkybox>& skybox) noexcept
{
  mSkybox = skybox;
}

void AAWorld::setRenderDistance(const float& amt, int camId)
{
  getCamera(camId).setRenderDistance(amt);

  // todo: then update projection matrices for effected shaders
}

void AAWorld::setToPerspectiveMouseHandling()
{
  DISPLAY->setCurorPosToPerspectiveCalc();

  //if (getMouseHandlingType() != MouseReporting::PERSPECTIVE)
  //{
  //}
  //else
  //{
  //  std::cout << "engine already in PERSPECTIVE mouse calc\n";
  //}
}

void AAWorld::setToStandardMouseHandling()
{
  DISPLAY->setCurorPosToStandardCalc();
}

void AAWorld::setWindowTitle(const char* title)
{
  DISPLAY->setWindowTitle(title);
}

void AAWorld::setCursorToEnabled(bool isHardwareRendered)
{
  if (isHardwareRendered)
  {
    DISPLAY->setCursorToHidden();
  }
  else
  {
    DISPLAY->setCursorToVisible();
  }
}

void AAWorld::setCursorToDisabled()
{
  DISPLAY->setCursorToDisabled();
}


void AAWorld::setRenderDistance(int camId, float amt)
{
  for (auto& cam : mCameras)
  {
    if (cam.getID() == camId)
    {
      cam.setRenderDistance(amt);
      return;
    }
  }

  // if code gets here there is an error, camId not found
  //std::cout << "cam not found for id [" << camId << "].\n";
}

void AAWorld::setProjectionMatrix(int shadId, int camId)
{
  getShader(shadId).use();
  getShader(shadId).setMat4("projection", getCamera(camId).getProjectionMatrix());
}
