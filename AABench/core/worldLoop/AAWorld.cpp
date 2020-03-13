#include "AAWorld.h"
#include "../winSys/AAControls.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

int AAWorld::runMainLoop()
{
  begin();
  while (!glfwWindowShouldClose(DISPLAY->getWindow()))
  {
    deltaUpdate();
    render();
    afterRenderUpdate();
  }
  //resetEngine();
  return 0;
}

void AAWorld::shutdown()
{
  //DISPLAY->resetStateDataToDefault();
  DISPLAY->closeWindow();
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

void AAWorld::setSkybox(const std::shared_ptr<AASkybox>& skybox) noexcept
{
  mSkybox = skybox;
}

void AAWorld::addToOnBegin(void(*function)())
{
  onBegin.push_back(function);
}

void AAWorld::addToDeltaUpdate(void(*function)(float))
{
  onDeltaUpdate.push_back(function);
}


void AAWorld::addToTimedOutKeyHandling(bool(*function)(AAKeyBoardInput&))
{
  onTimeoutKeyHandling.push_back(function);
}

void AAWorld::addToScrollHandling(void(*function)(AAScrollInput&))
{
  onScrollHandling.push_back(function);
}

void AAWorld::addToKeyHandling(void(*function)(AAKeyBoardInput&))
{
  onKeyHandling.push_back(function);
}

void AAWorld::addToMouseHandling(void(*function)(AAMouseInput&))
{
  onMouseHandling.push_back(function);
}

void AAWorld::addToUpdate(void(*function)())
{
  onUpdate.push_back(function);
}

void AAWorld::addToSlowUpdate(void(*function)())
{
  onSlowDeltaUpdate.push_back(function);
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
void AAWorld::setToPerspectiveMouseHandling()
{
  DISPLAY->setCurorPosToPerspectiveCalc();
}
void AAWorld::setToStandardMouseHandling()
{
  DISPLAY->setCurorPosToStandardCalc();
}
void AAWorld::setWindowTitle(const char* title)
{
  DISPLAY->setWindowTitle(title);
}
void AAWorld::setRenderDistance(int camId, float amt) noexcept
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










