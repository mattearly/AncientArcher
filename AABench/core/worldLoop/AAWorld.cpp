#include "AAWorld.h"
#include "../winSys/AAControls.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>

int AAWorld::runMainLoop()
{
  begin();
  while (!glfwWindowShouldClose(DISPLAY->getWindow()))
  {
    static float currentFrame(0.f), deltaTime(0.f), lastFrame(0.f);
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    mEngineRunTimer += deltaTime;
    update(deltaTime);
    render();
    update();
  }
  resetEngine();
  return 0;
}

void AAWorld::shutdown()
{
  //resetData();
  //DISPLAY->resetStateDataToDefault();
  DISPLAY->closeWindow();
}

MouseReporting AAWorld::getMouseHandlingType()
{
  return DISPLAY->mMouseReporting;
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

  // set proj matries hack from first cam
  for (auto& shad : mShaders)
  {
    //shad.setMat4("projection", mCameras.front().getProjectionMatrix());
    const int sID = shad.getID();
    std::cout << "shad id: " << sID << '\n';

    const int cID = mCameras.front().getID();
    std::cout << "shad id: " << sID << '\n';

    setProjectionMatrix(sID, cID);  //set shader (sID) from cam (cID)
  }

  // if there is a skybox
  if (mSkybox) 
  {
    // if there is a camera
    if (mCameras.size() > 0) {
      // se the projection matrix on the skybox from the first cam proj matrix
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
}

void AAWorld::update()
{
  processSystemKeys();
  const float passedTime = mEngineRunTimer - mTimeOutCheckStamp;

  mButtonTimeOutSoFar += passedTime;
  if (mButtonTimeOutSoFar > mKeyTimeOutLength)   //todo: move to delta update
  {
    int buttonUsed = false;
    if ((AAControls::getInstance()->mButtonState.leftAlt || AAControls::getInstance()->mButtonState.rightAlt) && AAControls::getInstance()->mButtonState.enter)
    {
      DISPLAY->toggleFullScreen();
      buttonUsed = true;
    }

    for (auto& oTKH : onTimeoutKeyHandling)
    {
      if (oTKH(AAControls::getInstance()->mButtonState))
      {
        buttonUsed = true;
      }
    }

    if (buttonUsed)
    {
      mButtonTimeOutSoFar = 0.f;
    }
  }

  for (auto& oKH : onKeyHandling)
  {
    oKH(AAControls::getInstance()->mButtonState);
  }

  for (auto& oSH : onScrollHandling)
  {
    oSH(AAControls::getInstance()->mMouseWheelScroll);
  }

  AAControls::getInstance()->mMouseWheelScroll.xOffset = 0;
  AAControls::getInstance()->mMouseWheelScroll.yOffset = 0;

  for (auto& oMH : onMouseHandling)
  {
    oMH(AAControls::getInstance()->mMousePosition);
  }

  if (getMouseHandlingType() == MouseReporting::PERSPECTIVE)
  {
    AAControls::getInstance()->mMousePosition.xOffset = 0;
    AAControls::getInstance()->mMousePosition.yOffset = 0;
  }

  for (auto& oU : onUpdate)
  {
    oU();
  }

  static float worldUpdateTimeOutSoFar = 0;
  worldUpdateTimeOutSoFar += passedTime;
  if (worldUpdateTimeOutSoFar > mSlowUpdateDelay)
  {
    for (auto& oSU : onSlowUpdate)// todo: move to delta update
    {
      oSU();
    }
    worldUpdateTimeOutSoFar = 0.f;
  }

  mTimeOutCheckStamp = mEngineRunTimer;

}

void AAWorld::processSystemKeys()
{
  AAControls::getInstance()->pullButtonStateEvents();
}

void AAWorld::initEngine()
{
  initDisplay();
  mEngineRunTimer = 0.f;
  mSlowUpdateDelay = 1.f;
  mKeyTimeOutLength = 0.56667f;
  mTimeOutCheckStamp = 0.f;
  mButtonTimeOutSoFar = 0.f;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context) -- must be after initDisplay
  {
    std::cout << "failed to init glad @ file " __FILE__ << " line " << __LINE__ << '\n';
    exit(-1);
  }

  //AAViewport::getInstance()->resetViewportVars();

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
  onSlowUpdate.clear();

  // reset all state data
  mEngineRunTimer = 0.f;
  mSlowUpdateDelay = 1.f;
  mKeyTimeOutLength = 0.56667f;
  mTimeOutCheckStamp = 0.f;
  mButtonTimeOutSoFar = 0.f;

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
  std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get const AACamera\n";
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
  std::cout << "cam ID by the ID [" << camId << "] was not found. unable to get AACamera\n";
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
  std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
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
  std::cout << "shad ID by the ID [" << shadId << "] was not found.\n";
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
  std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
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
  std::cout << "game object ID by the ID [" << objId << "] was not found.\n";
  exit(-1);
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
  onSlowUpdate.push_back(function);
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

void AAWorld::setToPerspectiveMouseHandling()
{
  if (getMouseHandlingType() != MouseReporting::PERSPECTIVE)
  {
    DISPLAY->setCurorPosToPerspectiveCalc();
  }
  else
  {
    std::cout << "engine already in PERSPECTIVE mouse calc\n";
  }
}

void AAWorld::setToStandardMouseHandling()
{
  if (getMouseHandlingType() != MouseReporting::STANDARD)
  {
    DISPLAY->setCurorPosToStandardCalc();
  }
  else
  {
    std::cout << "engine already in STANDARD mouse calc\n";
  }
}

void AAWorld::setKeyTimeoutLength(float time) noexcept
{
  mKeyTimeOutLength = time < 0.f ? 0.0001f : time;
}

void AAWorld::setWindowTitle(const char* title)
{
  DISPLAY->setWindowTitle(title);
}


void AAWorld::setCursorToDisabled()
{
  DISPLAY->setCursorToDisabled();
}

void AAWorld::setCursorToEnabled()
{
  DISPLAY->setCursorToEnabled();
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
  std::cout << "cam not found for id [" << camId << "].\n";
}

void AAWorld::setProjectionMatrix(int shadId, int camId)
{
  getShader(shadId).use();
  getShader(shadId).setMat4("projection", getCamera(camId).getProjectionMatrix());
}
