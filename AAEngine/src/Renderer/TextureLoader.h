#pragma once
#include "../../include/AncientArcher/Types.h"
#include "TextureInfo.h"
#include <assimp/material.h>
#include <assimp/texture.h>
#include <assimp/scene.h>
#include <unordered_map>

namespace AA{
namespace ModelLoader{
extern string mModelDir;
extern string mModelFileName;
extern string mModelFileExtension;
}
namespace TextureLoader{

u32 LoadTexture(const string& texture_path);

int loadMaterialTextures(const aiScene* scn, const aiMaterial* mat, aiTextureType type, std::string typeName, std::unordered_map<u32, string>& out_texInfo);

}  // end namespace TextureLoader
}  // end namespace AA
