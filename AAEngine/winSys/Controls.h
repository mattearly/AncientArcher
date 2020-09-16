/*
Controls
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
#include "Input.h"

namespace AA
{

enum class STANDARDMOUSEZEROS { DEFAULT, TOP_LEFT_0_to_1, BOT_LEFT_0_to_1, TOP_LEFT_FULL_RANGE, BOT_LEFT_FULL_RANGE };

class Controls
{
public:

  static Controls* getInstance();

  float getFPPMouseSensitivity() const noexcept;
  void setFPPMouseSensitivity(float sensitivity) noexcept;

  friend class Display;      // for access to mouseMovement and mouseScrollWheelMovement
  friend class Loop;    // for access to Input

private:

  STANDARDMOUSEZEROS mStandardMouseZeros = STANDARDMOUSEZEROS::BOT_LEFT_0_to_1;

  void pullButtonStateEvents();

  KeyboardInput mButtonState;
  MouseInput    mMousePosition;
  ScrollInput   mMouseWheelScroll;

  float mFPPMouseSensitivity = 0.1f;  ///< mouse sensitivity while in first person perspective
  bool mRenewFPP = true;

  void perspectiveMouseMovement(float xpos, float ypos) noexcept;
  void standardMouseMovement(float xpos, float ypos);
  void mouseScrollWheelMovement(float xpos, float ypos) noexcept;

  void resetControlVars() noexcept;

};

}  // end namespace AA