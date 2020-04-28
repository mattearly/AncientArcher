/*
Loop
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.
* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/

#pragma once
#include <glad\glad.h>
#include "winSys/Display.h"
#include "winSys/Input.h"
#include "winSys/Controls.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameObject.h"
#include <vector>
#include <functional>
#include <memory>

namespace AA
{
#define DISPLAY        Display::getInstance()
#define CONTROLS       Controls::getInstance()
#define PointerXOffset Controls::getInstance()->mMousePosition.xOffset
#define PointerYOffset Controls::getInstance()->mMousePosition.yOffset
class Loop
{
public:
  int runMainLoop();
  void shutdown();

  Loop();
  Loop(const char* title);

  const Camera& getCamera(int camId) const;
  Camera& getCamera(int camId);

  const OGLShader& getShader(int shadId) const;
  OGLShader& getShader(int shadId);

  const GameObject& getGameObject(int objId) const;
  GameObject& getGameObject(int objId);

  const MouseReporting getMouseReportingMode() const;
  const int getCursorMode() const;

  int addCamera();
  int addShader(const char* vert_path, const char* frag_path);
  int addObject(const char* path, int cam_id, int shad_id);
  int addObject(const char* path, int cam_id, int shad_id, const std::vector<InstanceDetails>& details);

  void setSkybox(const std::shared_ptr<Skybox>& skybox) noexcept;

  void addToOnBegin(void(*function)());
  void addToDeltaUpdate(void(*function)(float));
  void addToTimedOutKeyHandling(bool(*function)(KeyboardInput&));
  void addToKeyHandling(void(*function)(KeyboardInput&));
  void addToScrollHandling(void(*function)(ScrollInput&));
  void addToMouseHandling(void(*function)(MouseInput&));
  void addToUpdate(void(*function)());
  void addToSlowUpdate(void(*function)());

  void setCursorToEnabled(bool isHardwareRendered = false);
  void setCursorToDisabled();
  void setToPerspectiveMouseHandling();
  void setToStandardMouseHandling();
  void setWindowTitle(const char* title);
  void setRenderDistance(int camId, float amt) noexcept;
  void setProjectionMatrix(int shadId, int camId);

  friend class Controls;

private:

  float mLastFrameTime;                                                     ///< keeps track of the previous frame's time
  float mCurrentFrameTime;                                                  ///< keeps track of the current frame's time
  float mDeltaTime;                                                         ///< the delta difference between previous and current frame
  float mNonSpammableKeysTimeout;                                           ///< keeps track of how long the keys have timed out
  float mNoSpamWaitLength;                                                  ///< how long the non-spammable keys are to time out for at least
  float mSlowUpdateTimeout;                                                 ///< keeps track of how how long the slow update has been timed out
  float mSlowUpdateWaitLength;                                              ///< ms length the slow update times out for at least

  std::vector<Camera>     mCameras;                                       ///< list of available cameras
  std::vector<OGLShader>  mShaders;                                       ///< list of available shaders
  std::vector<GameObject> mGameObjects;                                   ///< list of available objects
  std::shared_ptr<Skybox> mSkybox;                                        ///< the main skybox

  std::vector<std::function<void()> >                 onBegin;              ///< list of functions to run once when runMainLoop is called
  std::vector<std::function<void(float)> >            onDeltaUpdate;        ///< list of functions that rely on deltatime in the main loop
  std::vector<std::function<void()> >                 onUpdate;             ///< list of functions that run every frame in the main loop
  std::vector<std::function<void()> >                 onSlowDeltaUpdate;    ///< list of functions to run every only every mSlowUpdateWaitLength in the main loop
  std::vector<std::function<void(KeyboardInput&)> > onKeyHandling;        ///< list of functions to handle keypresses every frame in the main loop
  std::vector<std::function<void(ScrollInput&)> >   onScrollHandling;     ///< list of functions to handle mouse wheel scroll every frame in the main loop
  std::vector<std::function<void(MouseInput&)> >    onMouseHandling;      ///< list of functions to handle mouse movement every frame in the main loop
  std::vector<std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling; ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press

  // called during runMainLoop
  void begin();
  void deltaUpdate();
  void render();

  // helpers
  void initEngine();
  void resetEngine() noexcept;
  void setProjectionMatToAllShadersFromFirstCam_hack();  //used for testing purposes until more elegant solution appears

};

}  // end namespace AA