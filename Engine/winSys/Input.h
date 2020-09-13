/*
Input
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

namespace AA
{
struct KeyboardInput
{
  //keyboard
  bool esc, f1, f2, f3, f4, f5, f6, f7, f8, f9,
    f10, f11, f12;

  bool graveAccent, n1, n2, n3, n4, n5, n6, n7,
    n8, n9, n0, minus, equal, backspace;

  bool a, b, c, d, e, f, g, h, i, j, k, l, m,
    n, o, p, q, r, s, t, u, v, w, x, y, z;

  bool tab, leftShift, rightShift, leftControl,
    rightControl, leftAlt, rightAlt, spacebar;

  bool leftSquareBracket, rightSquareBracket;

  bool backslash, semiColon, apostrophe, enter;

  bool comma, period, forwardSlash;

  bool printScreen, scrollLock, pauseBreak, insert,
    del, home, end, pageUp, pageDown;

  bool upArrow, downArrow, leftArrow, rightArrow;

  bool menu, leftSuper, rightSuper;

  // todo 
  // bool keypad_keys;

  //mouse buttons
  bool mouseButton1, mouseButton2, mouseButton3,
    mouseButton4, mousebutton5, mouseButton6,
    mousebutton7, mouseButton8;

};

struct MouseInput
{
  float xOffset;
  float yOffset;
};

struct ScrollInput
{
  float xOffset;
  float yOffset;
};

}  // end namespace AA