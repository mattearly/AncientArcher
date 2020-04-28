/*
Conversions
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

//#include "Conversions.h"
//namespace AA
//{
//glm::mat4 Conversions::aiMat4_to_glmMat4(const aiMatrix4x4& inMat)
//{
//  glm::mat4 outMat;
//  outMat[0][0] = inMat.a1;
//  outMat[1][0] = inMat.b1;
//  outMat[2][0] = inMat.c1;
//  outMat[3][0] = inMat.d1;
//  outMat[0][1] = inMat.a2;
//  outMat[1][1] = inMat.b2;
//  outMat[2][1] = inMat.c2;
//  outMat[3][1] = inMat.d2;
//  outMat[0][2] = inMat.a3;
//  outMat[1][2] = inMat.b3;
//  outMat[2][2] = inMat.c3;
//  outMat[3][2] = inMat.d3;
//  outMat[0][3] = inMat.a4;
//  outMat[1][3] = inMat.b4;
//  outMat[2][3] = inMat.c4;
//  outMat[3][3] = inMat.d4;
//  return outMat;
//}
//
//glm::vec3 Conversions::aiVec3_to_glmVec3(const aiVector3D& inVec) noexcept
//{
//  glm::vec3 outVec;
//  outVec.x = inVec.x;
//  outVec.y = inVec.y;
//  outVec.z = inVec.z;
//  return outVec;
//}
//
//glm::vec4 Conversions::aiColor4_to_glmVec4(const aiColor4D& inVec) noexcept
//{
//  glm::vec4 outVec;
//  
//  outVec.x = inVec.r;
//  outVec.y = inVec.g;
//  outVec.z = inVec.b;
//  outVec.w = inVec.a;
//
//  return outVec;
//}
//
//glm::quat Conversions::aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept
//{
//  glm::quat outQuat;
//  outQuat.w = inQuat.w;
//  outQuat.x = inQuat.x;
//  outQuat.y = inQuat.y;
//  outQuat.z = inQuat.z;
//  return outQuat;
//}
//}  // end namespace AA