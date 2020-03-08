#include "Conversions.h"
namespace mearly
{
glm::mat4 Conversions::aiMat4_to_glmMat4(const aiMatrix4x4& inMat)
{
  glm::mat4 outMat;
  outMat[0][0] = inMat.a1;
  outMat[1][0] = inMat.b1;
  outMat[2][0] = inMat.c1;
  outMat[3][0] = inMat.d1;
  outMat[0][1] = inMat.a2;
  outMat[1][1] = inMat.b2;
  outMat[2][1] = inMat.c2;
  outMat[3][1] = inMat.d2;
  outMat[0][2] = inMat.a3;
  outMat[1][2] = inMat.b3;
  outMat[2][2] = inMat.c3;
  outMat[3][2] = inMat.d3;
  outMat[0][3] = inMat.a4;
  outMat[1][3] = inMat.b4;
  outMat[2][3] = inMat.c4;
  outMat[3][3] = inMat.d4;
  return outMat;
}

glm::vec3 Conversions::aiVec3_to_glmVec3(const aiVector3D& inVec) noexcept
{
  glm::vec3 outVec;
  outVec.x = inVec.x;
  outVec.y = inVec.y;
  outVec.z = inVec.z;
  return outVec;
}

glm::vec4 Conversions::aiColor4_to_glmVec4(const aiColor4D& inVec) noexcept
{
  glm::vec4 outVec;
  
  outVec.x = inVec.r;
  outVec.y = inVec.g;
  outVec.z = inVec.b;
  outVec.w = inVec.a;

  return outVec;
}

glm::quat Conversions::aiQuat_to_glmQuat(const aiQuaternion& inQuat) noexcept
{
  glm::quat outQuat;
  outQuat.w = inQuat.w;
  outQuat.x = inQuat.x;
  outQuat.y = inQuat.y;
  outQuat.z = inQuat.z;
  return outQuat;
}
}  // end namespace mearly