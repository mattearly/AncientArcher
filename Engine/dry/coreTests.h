/*
coreTests
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

/*
  Tests for AncientArcherGameEngine.
*/
#pragma once
#include "addGodMovement.h"
#include "addSkybox.h"
#include "addRandomWorld.h"
#include <iostream>
#include <string>
#include "../Loop.h"
#include "../winSys/Input.h"
#include "../fbxLoader/FBXManager.h"
#include "FBXPrint.h"

int testFBXLoader()
{
  bool ret_code;
  FBXManager fbxmanager;
  ret_code = fbxmanager.LoadFBX("../assets/erikaarcher/erika_archer_bow_arrow.fbx");
  
  for (const auto& t : fbxmanager.mScenes)
  {
    PrintNode(t->GetRootNode());
  }

  return ret_code;
}

int testEngineLoopBase()
{
  LOOP->setWindowTitle("demo loop base");

  // Add to loop functions.
  LOOP->addToOnBegin([]()noexcept {printf("World Started. timestamp: %s \n", __TIMESTAMP__); });
  LOOP->addToDeltaUpdate([](float step)noexcept {
    static float timePassed = 0; timePassed += step; const float newcolor = abs(sin(timePassed));
    DISPLAY->setWindowClearColor(glm::vec3(newcolor, newcolor, newcolor)); 
    });
  LOOP->addToDeltaUpdate([](float step) {std::cout << ".      process delta update: " << step << '\n'; });
  LOOP->addToTimedOutKeyHandling([](AA::KeyboardInput& keys)noexcept {printf("..     %fms timedout keyboard/mouse\n", 90.f); return false;  });
  LOOP->addToKeyHandling([](AA::KeyboardInput& keys) {std::cout << "...    process keyboard/mouse \n";  });
  LOOP->addToScrollHandling([](AA::ScrollInput& scroll) {std::cout << "....   process mouse scroll: X:" << scroll.xOffset << " Y:" << scroll.yOffset << '\n'; });
  LOOP->addToMouseHandling([](AA::MouseInput& pointer) {std::cout << ".....  process mouse pointer: X:" << pointer.xOffset << " Y:" << pointer.yOffset << '\n';  });
  LOOP->addToUpdate([]() {std::cout << "...... process update\n"; });

  return LOOP->runMainLoop();
}

int testEngineDemoWorld()
{
  LOOP->setWindowTitle("Test Engine Demo World");
  DISPLAY->setWindowSize(800, 600, 910, 160);
  const int cameraId = setGodCamWithMovement();
  addRandomWorld(cameraId);
  addCubeSkybox();
  return LOOP->runMainLoop();
}

int runAllCoreTests()
{
  int result = 0;
  result = testFBXLoader();
  result = testEngineLoopBase();
  result = testEngineDemoWorld();
  return result;
}