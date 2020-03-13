#include "AAWorld.h"

// following 3 lines are for mouse pointer position debugging purposes
#include <iostream>
#include <iomanip>
#define showmousepos(x, y) std::cout << std::setprecision(2) << #x << "(" << x << ")," << #y << "(" << y << ")\n";

void AAWorld::begin()
{
  DISPLAY->keepWindowOpen();

  for (auto& oB : onBegin)
  {
    oB();
  }

  setProjectionMatToAllShadersFromFirstCam_hack();

}

void AAWorld::deltaUpdate()
{

  CONTROLS->pullButtonStateEvents();

  // update engine run delta times
  mCurrentFrameTime = static_cast<float>(glfwGetTime());
  mDeltaTime = mCurrentFrameTime - mLastFrameTime;
  mLastFrameTime = mCurrentFrameTime;

  // go through all updates that need access to delta time
  for (auto& oDU : onDeltaUpdate)
  {
    oDU(mDeltaTime);
  }

  // process keyboard input
  for (auto& oKH : onKeyHandling) { oKH(CONTROLS->mButtonState); }

  // absorb scroll wheel
  for (auto& oSH : onScrollHandling) { oSH(CONTROLS->mMouseWheelScroll); }

  // reset scroll wheel to 0's after processing scroll wheel
  CONTROLS->mMouseWheelScroll.xOffset = 0;
  CONTROLS->mMouseWheelScroll.yOffset = 0;

  // handle mouse position
  for (auto& oMH : onMouseHandling) { oMH(CONTROLS->mMousePosition); }


  // Snap cursor to the middle of the screen if it is in perspective and cursor is disabled (FPP mode)
  if (getMouseReportingMode() == MouseReporting::PERSPECTIVE && getCursorMode() == GLFW_CURSOR_DISABLED)
  {
    //deb(CONTROLS->mMousePosition.xOffset);
    //deb(CONTROLS->mMousePosition.yOffset);

    CONTROLS->mMousePosition.xOffset = 0;
    CONTROLS->mMousePosition.yOffset = 0;
  }
  else if (getMouseReportingMode() == MouseReporting::STANDARD && getCursorMode() == GLFW_CURSOR_NORMAL) // standard and normal
  {
    // dont snap the position
    showmousepos(PointerXOffset, PointerYOffset);
  }

  // run through user prefered updates
  for (auto& oU : onUpdate) { oU(); }

  // delayed updates for things you don't want spammed.
  // update accum time for delayed updates
  mSlowUpdateTimeout += mDeltaTime;
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
  mNonSpammableKeysTimeout += mDeltaTime;

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

    // draw using the shader for it
    obj.draw(getShader(shaderID));
  }

  // draw skybox if one was specified
  if (mSkybox) { mSkybox->render(mCameras.front()); }

  DISPLAY->swapWindowBuffers();
}

void AAWorld::initEngine()
{
  resetEngine();

  DISPLAY->initFromEngine();

  // Init OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // init glad (for opengl context) -- must be after initDisplay
  {
    //std::cout << "failed to init glad @ file " __FILE__ << " line " << __LINE__ << '\n';
    exit(-1);
  }
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

  mLastFrameTime = 0.f;
  mCurrentFrameTime = 0.f;
  mDeltaTime = 0.f;
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
