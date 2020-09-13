/*
Dice
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
#include "RandomNumbers.h"

// SIMPLIFIED SINGLE DICE ROLLERS
// real rolls - dice replicants  (only 1d)
#define roll1d2   AA::Random::ZOOR() + 1
#define roll1d3   AA::Random::NTKR(1, 3)
#define roll1d4   AA::Random::NTKR(1, 4)
#define roll1d6   AA::Random::NTKR(1, 6)
#define roll1d8   AA::Random::NTKR(1, 8)
#define roll1d10  AA::Random::NTKR(1, 10)
#define roll1d12  AA::Random::NTKR(1, 12)
#define roll1d20  AA::Random::NTKR(1, 20)
#define roll1d30  AA::Random::NTKR(1, 30)
#define roll1d34  AA::Random::NTKR(1, 34)
#define roll1d40  AA::Random::NTKR(1, 40)
#define roll1d60  AA::Random::NTKR(1, 60)
#define roll1d100 AA::Random::NTKR(1, 100)
#define roll1d120 AA::Random::NTKR(1, 120)
#define roll1d400 AA::Random::NTKR(1, 400)

// further aliases
#define rolld2   roll1d2
#define rolld3   roll1d3
#define rolld4   roll1d4
#define rolld6   roll1d6
#define rolld8   roll1d8
#define rolld10  roll1d10
#define rolld12  roll1d12
#define rolld20  roll1d20
#define rolld30  roll1d30
#define rolld34  roll1d34
#define rolld40  roll1d40
#define rolld60  roll1d60
#define rolld100 roll1d100
#define rolld120 roll1d120
#define rolld400 roll1d400
