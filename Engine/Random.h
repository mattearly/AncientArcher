/*
Random
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

// unatural rolls
#define roll1d1000      AA::Random::NTKR(1, 1000)      // 1 to 1,000   Thousand
#define roll1d10000     AA::Random::NTKR(1, 10000)     // 1 to 10,000  Ten Thousand
#define roll1d100000    AA::Random::NTKR(1, 100000)    // 1 to 100,000  Hundred Thousand
#define roll1d1000000   AA::Random::NTKR(1, 100000)    // 1 to 1,000,000  Million
#define roll1d10000000  AA::Random::NTKR(1, 1000000)   // 1 to 10,000,000  Ten Million
#define roll1d100000000 AA::Random::NTKR(1, 10000000)  // 1 to 100,000,000  Hundred Million

#define rolld1000      roll1d1000
#define rolld10000     roll1d10000
#define rolld100000    roll1d100000
#define rolld1000000   roll1d1000000
#define rolld10000000  roll1d10000000
#define rolld100000000 roll1d100000000
