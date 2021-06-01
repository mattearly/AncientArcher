#pragma once
#include "../../include/AncientArcher/Types.h"
namespace AA{
struct BoneInfo {
  int id;  // id is index in finalBoneMatrices 
  mat4 offset;  // offset matrix transforms vertex from model space to bone s pace 
};
}
