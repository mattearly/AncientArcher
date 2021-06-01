#include <../AncientArcher/Types.h>
#include <forward_list>
#include "TextureInfo.h"
#include <map>
#include "../Scene/BoneInfo.h"

namespace AA{

string mModelDir = "";
string mModelFileName = "";
string mModelFileExtension = "";

std::forward_list<TextureInfo> mLoadedTextures{};

int MAX_BONE_WEIGHTS = 4;
int m_BoneCounter = 0;


}